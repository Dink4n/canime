#ifndef CANIME_COMMON_H
#define CANIME_COMMON_H

#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

void die(const char *fmt, ...);
void warn(const char *fmt, ...);
void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
char *xstrdup(const char *str);

#endif /* CANIME_COMMON_H */
