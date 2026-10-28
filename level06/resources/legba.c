#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define CHUNK_SIZE 0x38
static struct gate_s {
    char _pad[8];
    size_t guard;
    void (*path)(void);
    char slack[32];
} g __attribute__((aligned(16)));
static char *claimed;
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
static void default_path(void) {
    puts("[LEGBA] the crossroads are quiet.");
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void cmd_mark(void) {
    printf("size: "); fflush(stdout);
    g.guard = (size_t)rd_long();
}
static void cmd_open(void) {
    printf("addr: "); fflush(stdout);
    free((void *)rd_long());
    puts("[LEGBA] a path opens.");
}
static void cmd_claim(void) {
    claimed = malloc(CHUNK_SIZE);
    printf("[LEGBA] claimed %p\n", (void *)claimed);
}
static void cmd_write(void) {
    if (!claimed) return;
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, claimed, CHUNK_SIZE);
}
static void cmd_cross(void) {
    g.path();
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    g.path = default_path;
    printf("\033[0;32m");
    printf("  [MATRIX] Baron Legba opens the way.\n");
    printf("  [LEGBA] Crossroads allocator v1.7\n");
    printf("\033[0m");
    printf("[LEGBA] veve @ %p\n", (void *)win);
    printf("[LEGBA] gate @ %p\n", (void *)&g.path);
    for (;;) {
        printf("\n[LEGBA] 1)mark 2)open 3)claim 4)write 5)cross 6)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: cmd_mark(); break;
            case 2: cmd_open(); break;
            case 3: cmd_claim(); break;
            case 4: cmd_write(); break;
            case 5: cmd_cross(); break;
            default: return 0;
        }
    }
}
