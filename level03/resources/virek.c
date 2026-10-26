#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_NODES 16
#define MAX_SIZE 0x800
static char *nodes[MAX_NODES];
static size_t node_sz[MAX_NODES];
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
static struct admin_s {
    char tag[24];
    void (*action)(void);
} g_admin;
static void default_action(void) {
    puts("[VIREK] node graph synchronized.");
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void node_new(void) {
    printf("size: "); fflush(stdout); size_t sz = (size_t)rd_long();
    if (sz == 0 || sz > MAX_SIZE) { puts("[VIREK] bad size."); return; }
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES) return;
    nodes[i] = malloc(sz);
    node_sz[i] = sz;
    printf("[VIREK] node allocated in slot %ld.\n", i);
}
static void node_del(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES || !nodes[i]) return;
    free(nodes[i]);
    puts("[VIREK] node released.");
}
static void node_edit(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES || !nodes[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, nodes[i], n);
}
static void node_view(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NODES || !nodes[i]) return;
    printf("[VIREK] node data: ");
    write(STDOUT_FILENO, nodes[i], node_sz[i]);
    puts("");
}
static void node_sync(void) {
    g_admin.action();
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    g_admin.action = default_action;
    strcpy(g_admin.tag, "TESSIER-ASHPOOL");
    printf("\033[0;34m");
    printf("  [TESSIER-ASHPOOL] Virek exists in a dozen cities simultaneously.\n");
    printf("  [VIREK] Distributed node manager v4.1\n");
    printf("\033[0m");
    printf("[VIREK] core @ %p\n", (void *)win);
    for (;;) {
        printf("\n[VIREK] 1)new 2)del 3)edit 4)view 5)sync 6)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: node_new(); break;
            case 2: node_del(); break;
            case 3: node_edit(); break;
            case 4: node_view(); break;
            case 5: node_sync(); break;
            default: return 0;
        }
    }
}
