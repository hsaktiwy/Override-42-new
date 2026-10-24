level00 was a challenge relate dto blind exploitation the goal was to open a shell inside it, the normal process is to reverse
engenner the executable to get some proper knowledge of it and define what could be vulnrable.
the process is named bobby and it executable exist at '/opt' with limited privelage:
```sh
level00@override:~$ ls  -la /opt/override/bobby
-rwx------ 1 flag00 flag00 14408 Jun 10 12:00 /opt/override/bobby
```
meaning that we can't follow the normal analysis process and we need to notice the behaviors of the executable process directly:
```sh
level00@override:~$ ps aux | grep bobby
flag00       726  0.0  0.0   2548  1328 ?        Ss   14:43   0:00 /opt/override/bobby
```
based on the intell we have the executable is running as service and expose to port ``4243``.
when trying to connect to the service called bobby using ``nc 0.0.0.0 4243`` we noticed the following behavior:
```sh
level00@override:~$ ps aux | grep bobby
flag00       726  0.0  0.0   2548  1328 ?        Ss   14:43   0:00 /opt/override/bobby
flag00      9140  0.0  0.0   2548   416 ?        S    15:23   0:00 /opt/override/bobby
```

hmm ! 2 process of bobby, meaning each time someone try to communicat with the bobby service the parent process fork a shild process for it!
this open the path to exploit the fact that a child process enhirit from his parent most of the binary protection randomized seeds, so if the parent have a canary protection active the
child inhiret the protection and the canary value it self, so if we find a buffer overflow and crash the child we can relay on the fact that if the child crashed or not we can bruteforce the 
canary out and using some trick arround the idea of a child crashed or didn't crash we can find our self  able to use the blindrop attack.

Quick and simplified recap for blind rop attack :
Blind Return-Oriented Programming (BROP) attack is a hacking technique that lets an attacker exploit a system without having access to the target program's source code or binary file. Traditional exploits require a hacker to read the application's code beforehand to find instructions. BROP bypasses this entirely by reading memory remotely through a trial-and-error "side-channel."
The Core Requirement (How it Relies on Forking)
For a BROP attack to work, the target application must use a forking architecture (like many web servers or daemons).
• The Loophole: When a main server process forks a child process to handle a network request, the child inherits the exact same memory layout and security protections (like ASLR and Stack Canaries) as the parent.
• The Exploit Method: If a child process crashes due to a bad guess by the attacker, the main server simply re-runs a new child with the exact same memory states.
This allows the attacker to crash the application thousands of times, guessing the hidden security seeds byte-by-byte until they bypass them.

And voila we know what attack we will be trying but before anything else let build our assumptions first, we will start with the eazier case of the blind rop attack and the eazier assumption which in later we proved that she is the one we are dealing with .
Using blind rop attack we can execute the executable internal functions with eaze specialy the one that doesn't require arguments, as it want need from us to find the proper gadget for them , so just by knowing it entry address of teh function we can call it , and like that we can build the assumtpion that there is a function insidde the executable that call the /bin/sh for us , we may need to find a ret gadget to make sure that the call will work properly as the system function require the stack to be aligned by 1 due to the usage of a SIMD instuction in it, and like that we need to find a ret gadget but first thing first.

in blind rop attack there is a tool that we need to use a tool that allowed this kind of attack to exist, which is :
``Stop Gadget`` :  An address in the code that pauses execution or keeps the connection open (like an infinite loop or a blocking network call).
While guessing memory locations, a successful guess usually causes a crash or process termination we can say which one is when we are using script , or at least it is hard to know. The attacker places a Stop Gadget immediately after a guess. If the server doesn't crash and keeps the connection alive, the attacker knows their previous guess was correct, and like that we know that the stop gadget can be anything that help our service to stay up.

so the idea and implementation is clear as the sun in the rainy day, everything is perfect!
Yes finding the padding offset to cause teh child proces to crash was eazy , spoiler alert it was 72, and building a simple bruteforce that find the canary was also was eazy
```py
# BYTE-BYTE Brutforce checker
def check_b_crash(payload, b, debug=False):
    try:
        message = payload + bytes([b])
        if debug:
            print('[<] DEBUG START!')
        p = remote(taddr, tport)
        # RECEIVE THE IDL ENTRY 
        p.recvuntil(PROMPT, timeout=TIMEOUT)
        # SEND MESSAGE           
        p.send(message)
        # check if any data is recieved
        res = p.recvline(timeout=TIMEOUT)
        if debug:
            print('[<] DEBUG: ', res)
        p.sendline(b"PING") # testing if the connection still there
        check = p.recv(timeout=TIMEOUT)
        if debug:
            print('[<] DEBUG: ', check)
        p.close()
        if check != b'':
            return True
        if debug:
            print('[<] DEBUG END!')
        return False
    except (EOFError, PwnlibException):
        p.close()
        return False

# brte for canary
canary = b'\x00'
building = C*padding + canary
if search_canary:
    print("[>] BruteForce canary ...")
    canary += bruteforce_nbyte(building, 7)
```
but finding the proper rip of the gadgets and the win function (the function that call shell and that we build our assumption  over it)
is not something that eazier as we will have alot of false positive (some signes that say we found it).

1) STOPGadget
to be honest i was going to wast too much ime thinking about, how to garenty that we found a stop gadget, but at the end i found away arround it , was thinking a stop gadget is a gadget that hold our connection with the child,
so i thought what if i found an address that trow me back to the infinit loop of bobby input receiving logic, i mena she is a loop after all and like that i build a small search to find a rip address with changing only teh first 2 byte of the rip and see if i can finnd where is the address range that throw me back to bobby loop:
```sh
level00@override:~$ nc 0.0.0.0 4243
[MATRIX] Bobby Newmark — unauthorized access detected.
[BOBBY] You don't know what you've gotten into: My input         
[BOBBY] Input received.
[MATRIX] Bobby Newmark — unauthorized access detected.
[BOBBY] You don't know what you've gotten into:
```
which i found in after i was randomly trying to bruteforce the rip and fall to it : STOP_GADGET = 0x00000000004014d3, yes we can find more stop gagdets , but i decided to chose this, at least for my first solve attemp.
else we can try to brute from from 0x1000 to 0x1500 or 0xffff and find the address that cause us a infinit loop.

now we found the stop gadget we will try to find a ret gadget the main idea is that ``ret`` is a assembly instruction that is equal to ``pop rip`` meaning that we will get  8 byte from teh top of the stack and put it in the ``rip``
which we will try to derefference and execute 