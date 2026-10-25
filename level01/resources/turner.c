#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define TARGET_SIZE 96
#define PROFILE_SIZE 32
#define MAX_TARGETS 6
typedef struct {
    unsigned int id;
    unsigned int priority;
    unsigned int status;
    char codename[PROFILE_SIZE];
    void (*brief)(unsigned int);
} target_t;
static target_t targets[MAX_TARGETS];
static unsigned int target_count = 0;
static void default_brief(unsigned int id) {
    printf("[TURNER] Target %u: no briefing available.\n", id);
}
static void init_targets(void) {
    static const char *names[] = {
        "ALPHA", "BRAVO", "CHARLIE", "DELTA", "ECHO", "FOXTROT"
    };
    for (unsigned int i = 0; i < MAX_TARGETS; i++) {
        targets[i].id = i;
        targets[i].priority = MAX_TARGETS - i;
        targets[i].status = 0;
        strncpy(targets[i].codename, names[i], PROFILE_SIZE - 1);
        targets[i].brief = default_brief;
    }
    target_count = MAX_TARGETS;
}
static void print_banner(void) {
    printf("\033[0;31m");
    printf("  [HOSAKA] Turner online. One shot.\n");
    printf("  [HOSAKA] Target acquisition system v3.0\n");
    printf("\033[0m");
}
static void info_leak(void) {
    printf("[TURNER] brief@binary: %p\n", (void *)default_brief);
    printf("[TURNER] libc_start: %p\n", (void *)exit);
}
static void acquire_target(void) {
    char profile[TARGET_SIZE];
    info_leak();
    printf("[TURNER] Target: ");
    fflush(stdout);
    gets(profile);
    for (unsigned int i = 0; i < target_count; i++) {
        if (strncmp(profile, targets[i].codename, PROFILE_SIZE) == 0) {
            targets[i].status = 1;
            printf("[TURNER] Target %u acquired.\n", i);
            targets[i].brief(i);
            return;
        }
    }
    printf("[TURNER] Target not found.\n");
}
int main(void) {
    setreuid(geteuid(), geteuid());
    init_targets();
    print_banner();
    acquire_target();
    return 0;
}
