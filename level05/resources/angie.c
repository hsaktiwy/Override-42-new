#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define DATA_SIZE 40
#define MAX_SIGNALS 16
typedef struct signal_s {
    void (*process)(struct signal_s *);
    char data[DATA_SIZE];
} signal_t;
static signal_t *signals[MAX_SIGNALS];
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
static void default_process(signal_t *s) {
    printf("[ANGIE] signal processed: %.8s\n", s->data);
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void capture(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS) return;
    signals[i] = malloc(sizeof(signal_t));
    signals[i]->process = default_process;
    memset(signals[i]->data, 0, DATA_SIZE);
    printf("[ANGIE] signal captured in slot %ld.\n", i);
}
static void release(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS || !signals[i]) return;
    free(signals[i]);
    puts("[ANGIE] signal released.");
}
static void write_signal(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS || !signals[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    if (n > sizeof(signal_t)) n = sizeof(signal_t);
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, signals[i], n);
}
static void process_signal(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_SIGNALS || !signals[i]) return;
    signals[i]->process(signals[i]);
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("\033[0;36m");
    printf("  [CONTINUITY] Angie dreams in code she didn't write.\n");
    printf("  [ANGIE] Neural signal processor v2.3\n");
    printf("\033[0m");
    printf("[ANGIE] kernel @ %p\n", (void *)win);
    for (;;) {
        printf("\n[ANGIE] 1)capture 2)release 3)write 4)process 5)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: capture(); break;
            case 2: release(); break;
            case 3: write_signal(); break;
            case 4: process_signal(); break;
            default: return 0;
        }
    }
}
