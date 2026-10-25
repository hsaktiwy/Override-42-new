#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define INPUT_SIZE 96
#define ITEM_SIZE 32
#define MAX_ITEMS 12
#define STASH_SIZE 512
typedef struct {
    unsigned int id;
    unsigned int value;
    unsigned int flags;
    char name[ITEM_SIZE];
} item_t;
static item_t stash[MAX_ITEMS];
static unsigned int stash_count = 0;
static char overflow_buf[STASH_SIZE];
static void init_stash(void) {
    static const char *names[] = {
        "ICE_DECK", "CYBERSPACE_MAP", "ICEBREAKER",
        "CREDCHIP", "WETWARE_JACK", "CONSTRUCT_ROM",
        "NEURAL_SPIKE", "MICROSOFTS", "SIMSTIM_UNIT",
        "BLACK_ICE", "FLATLINE_ROM", "MATRIX_KEY"
    };
    for (unsigned int i = 0; i < MAX_ITEMS; i++) {
        stash[i].id = i;
        stash[i].value = (i + 1) * 0x100;
        stash[i].flags = i % 4;
        strncpy(stash[i].name, names[i], ITEM_SIZE - 1);
    }
    stash_count = MAX_ITEMS;
}
__asm__(
    ".text\n"
    ".globl _finn_gadgets\n"
    "_finn_gadgets:\n"
    "    pop %rdi\n"
    "    pop %rsi\n"
    "    pop %rdx\n"
    "    ret\n"
);
static void print_banner(void) {
    printf("\033[0;33m");
    printf("  [JAMMER'S] The Finn has what you need.\n");
    printf("  [FINN] Name your price.\n");
    printf("\033[0m");
}
static void list_items(void) {
    for (unsigned int i = 0; i < stash_count; i++)
        printf("[FINN] [%02u] %-20s value=%04x flags=%u\n",
            stash[i].id, stash[i].name,
            stash[i].value, stash[i].flags);
}
static void handle_transaction(void) {
    char request[INPUT_SIZE];
    list_items();
    printf("[FINN] Request: ");
    fflush(stdout);
    gets(request);
    if (strncmp(request, "BUY:", 4) == 0) {
        unsigned int id = (unsigned int)strtoul(request + 4, NULL, 10);
        if (id < stash_count)
            printf("[FINN] Sold: %s\n", stash[id].name);
        else
            printf("[FINN] Not in stock.\n");
    } else if (strncmp(request, "INFO:", 5) == 0) {
        unsigned int id = (unsigned int)strtoul(request + 5, NULL, 10);
        if (id < stash_count)
            printf("[FINN] %s — value %04x flags %u\n",
                stash[id].name, stash[id].value, stash[id].flags);
    } else {
        printf("[FINN] Unknown request.\n");
    }
}
int main(void) {
    setreuid(geteuid(), geteuid());
    init_stash();
    print_banner();
    handle_transaction();
    return 0;
}
