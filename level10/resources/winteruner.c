#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define DATA_SIZE 32
#define MAX_OBJS 16
typedef struct obj_s {
    void (*finalize)(struct obj_s *);
    unsigned long id;
    char data[DATA_SIZE];
} obj_t;
static obj_t *heap[MAX_OBJS];
static unsigned int obj_seq = 0;
static void (*g_final)(void) __attribute__((aligned(16)));
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
// static void default_finalize(obj_t *o) {
//     printf("[3JANE] object %lu reclaimed.\n", o->id);
// }
static void unknown(void) {
    // puts("[3JANE] construct state committed.");
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void spawn(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS) return;
    heap[i] = malloc(sizeof(obj_t));
    heap[i]->finalize = unknown;
    heap[i]->id = obj_seq++;
    memset(heap[i]->data, 0, DATA_SIZE);
    printf("[W/N] node spawned.");
}
static void sever(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS || !heap[i]) return;
    free(heap[i]);
}
static void inscribe(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS || !heap[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    // if (n > sizeof(obj_t)) n = sizeof(obj_t);
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, heap[i], n);
}
static void probe(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS || !heap[i]) return;
    write(STDOUT_FILENO, heap[i], sizeof(obj_t));
    puts("");
}
static void merge(void) {
    g_final();
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    g_final = unknown;
    printf("\033[0;37m");
    printf("[MATRIX] Wintermute. Neuromancer. We are one.\n");
    printf("[W/N] You should not exist.\n");
    printf("\033[0m");
    for (;;) {
        printf("\n[W/N] 1)spawn 2)sever 3)inscribe 4)probe 5)merge 6)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: spawn(); break;
            case 2: sever(); break;
            case 3: inscribe(); break;
            case 4: probe(); break;
            case 5: merge(); break;
            default: return 0;
        }
    }
}
