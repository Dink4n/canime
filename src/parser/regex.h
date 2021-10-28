#ifndef CANIME_PARSER_H
#define CANIME_PARSER_H

#define MAX_REGEX_MATCHES 16
#define MAX_REGEX_MATCH_SIZE 256

#include <regex.h>
#include <stdint.h>

struct SubstrPos {
    int32_t start;
    int32_t end;
};

struct RegexResults {
    int32_t count;
    char matches[MAX_REGEX_MATCHES][MAX_REGEX_MATCH_SIZE];
};

// Find the first occurrence of pattern in buffer
char *regex_find(char *buffer, char *pattern);

// Find n occurences of pattern in buffer
struct RegexResults *regex_findall(char *buffer, char *pattern);

#endif /* CANIME_PARSER_H */
