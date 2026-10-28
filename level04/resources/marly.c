#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define TITLE_SIZE 40
#define MAX_PIECES 16
typedef struct piece_s {
    void (*render)(struct piece_s *);
    char title[TITLE_SIZE];
} piece_t;
static piece_t *gallery[MAX_PIECES];
__attribute__((used)) static void win(void) {
    system("/bin/sh");
}
static void show_title(piece_t *p) {
    printf("[MARLY] piece: %s\n", p->title);
}
static long rd_long(void) {
    char b[32];
    ssize_t n = read(STDIN_FILENO, b, sizeof(b) - 1);
    if (n <= 0) exit(0);
    b[n] = 0;
    return strtol(b, NULL, 0);
}
static void acquire(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES) return;
    gallery[i] = malloc(sizeof(piece_t));
    gallery[i]->render = show_title;
    memset(gallery[i]->title, 0, TITLE_SIZE);
    printf("[MARLY] acquired piece in slot %ld.\n", i);
}
static void annotate(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES || !gallery[i]) return;
    printf("len: "); fflush(stdout); size_t n = (size_t)rd_long();
    printf("text: "); fflush(stdout);
    read(STDIN_FILENO, gallery[i]->title, n);
}
static void display(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES || !gallery[i]) return;
    gallery[i]->render(gallery[i]);
}
static void deaccession(void) {
    printf("slot: "); fflush(stdout); long i = rd_long();
    if (i < 0 || i >= MAX_PIECES || !gallery[i]) return;
    free(gallery[i]);
    gallery[i] = NULL;
}
int main(void) {
    setreuid(geteuid(), geteuid());
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("\033[0;35m");
    printf("  [GALERIE DUPEREY] Marly examines the piece.\n");
    printf("  [MARLY] Something is wrong here.\n");
    printf("\033[0m");
    printf("[MARLY] curator @ %p\n", (void *)win);
    for (;;) {
        printf("\n[MARLY] 1)acquire 2)annotate 3)display 4)deaccession 5)quit\n> ");
        fflush(stdout);
        switch (rd_long()) {
            case 1: acquire(); break;
            case 2: annotate(); break;
            case 3: display(); break;
            case 4: deaccession(); break;
            default: return 0;
        }
    }
}
