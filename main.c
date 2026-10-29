#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 4244
#define DATA_SIZE 64
#define MAX_STREAMS 16
#define TITLE_SIZE 40
#define MAX_PIECES 16

typedef struct piece_s {
    void (*render)(struct piece_s *);
    char title[TITLE_SIZE];
} piece_t;

struct vtable {
    void (*flush)(struct stream *);
    void (*close)(struct stream *);
};
struct stream {
    struct vtable *vt;
    char data[DATA_SIZE];
};
static struct stream *streams[MAX_STREAMS];

int main(){
    char* ptr1 = malloc(sizeof(piece_t));
    char* ptr2 = malloc(sizeof(piece_t));
    //ptr 1 : 0x569db0b8f2a0, ptr2 : 0x569db0b8f2e0, (struct size 48)diff: 64
    printf("ptr 1 : %p, ptr2 : %p, (struct size %d) diff: %ld\n", ptr1, ptr2, sizeof(piece_t), (ptr2 - ptr1));
    printf("headers : %lx\n", *(long *)(ptr1+sizeof(piece_t)+8));
    free(ptr1);
    free(ptr2);
    ptr1 = malloc(sizeof(struct stream));
    ptr2 = malloc(sizeof(struct stream));
    printf("ptr 1 : %p, ptr2 : %p, (struct size %d) diff: %ld\n", ptr1, ptr2, sizeof(struct stream), (ptr2 - ptr1));
    printf("headers size : %lx\n", *(long *)(ptr1+sizeof(struct stream)));
    printf("headers size+8 : %lx\n", *(long *)(ptr1+sizeof(struct stream)+8));

    free(ptr1);
    free(ptr2);
}