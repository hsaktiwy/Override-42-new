#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_NOTES 16
struct fmt_ops {
    void (*emit)(const char *);
};
static struct fmt_ops *g_ops;
static char *notes[MAX_NOTES];
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
static void default_emit(const char *s) {
    printf("[HUSK] %s\n", s);
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void cmd_reset(void) {
    free(g_ops);
    puts("[HUSK] format engine reset.");
}
static void cmd_new(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NOTES) return;
    notes[i] = malloc(sizeof(struct fmt_ops));
    printf("[HUSK] note %ld ready.\n", i);
}
static void cmd_write(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_NOTES || !notes[i]) return;
    printf("bytes: "); fflush(stdout);
    read(STDIN_FILENO, notes[i], sizeof(struct fmt_ops));
}
static void cmd_format(void) {
    g_ops->emit("husk");
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    g_ops = malloc(sizeof(struct fmt_ops));
    g_ops->emit = default_emit;
    printf("\033[0;33m");
    printf("  [MATRIX] The husk remains.\n");
    printf("  [HUSK] Format processor v1.3\n");
    printf("\033[0m");
    printf("[HUSK] codec @ %p\n", (void *)win);
    for (;;) {
        printf("\n[HUSK] 1)reset 2)new 3)write 4)format 5)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: cmd_reset(); break;
            case 2: cmd_new(); break;
            case 3: cmd_write(); break;
            case 4: cmd_format(); break;
            default: return 0;
        }
    }
}
