#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "common.h"
#include "colors.h"

void die(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fputs(C_BRED, stderr);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    fputs(C_RESET, stderr);

    va_end(args);
    exit(1);
}

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) {
        die("malloc: %s", strerror(errno));
    }

    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    void *tmp = realloc(ptr, size);
    if (tmp == NULL) {
        die("realloc: %s", strerror(errno));
    }

    return tmp;
}

char *xstrdup(const char *str)
{
    char *tmp = strdup(str);
    if (tmp == NULL) {
        die("strdup: %s", strerror(errno));
    }

    return tmp;
}
