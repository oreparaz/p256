#include "system_info.h"
#include <stdio.h>

static void fun(int *caller)
{
    int local;
    if (caller < &local)
        printf("upward\n");
    else
        printf("downward\n");
}

void system_info(void) {
    int local;

    printf("stack grows: ");
    fun(&local);

    printf("endian: ");
    int n = 1;
    if (*(char *)&n == 1) printf("little\n");
    else printf("big\n");

    printf("sizeof(void*): %ld\n", sizeof(void*));
}
