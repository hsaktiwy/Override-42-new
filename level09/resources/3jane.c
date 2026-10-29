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
static void default_finalize(obj_t *o) {
    printf("[3JANE] object %lu reclaimed.\n", o->id);
}
static void default_commit(void) {
    puts("[3JANE] construct state committed.");
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void obj_new(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS) return;
    heap[i] = malloc(sizeof(obj_t));
    heap[i]->finalize = default_finalize;
    heap[i]->id = obj_seq++;
    memset(heap[i]->data, 0, DATA_SIZE);
    printf("[3JANE] object instantiated in slot %ld.\n", i);
}
static void obj_release(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS || !heap[i]) return;
    free(heap[i]);
}
static void obj_edit(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS || !heap[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    if (n > sizeof(obj_t)) n = sizeof(obj_t);
    printf("data: "); fflush(stdout);
    read(STDIN_FILENO, heap[i], n);
}
static void obj_view(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_OBJS || !heap[i]) return;
    printf("[3JANE] trace: ");
    write(STDOUT_FILENO, heap[i], sizeof(obj_t));
    puts("");
}
static void obj_commit(void) {
    g_final();
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    g_final = default_commit;
    printf("\033[0;37m");
    printf("  [VILLA STRAYLIGHT] 3Jane returns.\n");
    printf("  [3JANE] Object lifecycle manager v3.0\n");
    printf("\033[0m");
    for (;;) {
        printf("\n[3JANE] 1)new 2)release 3)edit 4)view 5)commit 6)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: obj_new(); break;
            case 2: obj_release(); break;
            case 3: obj_edit(); break;
            case 4: obj_view(); break;
            case 5: obj_commit(); break;
            default: return 0;
        }
    }
}
