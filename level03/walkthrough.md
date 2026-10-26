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

