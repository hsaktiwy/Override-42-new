// this file gather our assumption about bobby:
void handle_connection() {
    char buf[64];
    gets(buf);                           // 1. You send 72 'a's (overflows buf)
    puts("[BOBBY] Input received.");    // 2. Executes and sends text back to client
    return;                              // 3. Function returns -> Pops bad EIP -> SIGSEGV!
}