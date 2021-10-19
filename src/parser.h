#ifndef CANIME_PARSER_H
#define CANIME_PARSER_H

#define MAX_PARSER_MATCHES 16
#define MAX_PARSER_MATCH_SIZE 256

#include <regex.h>
#include <stdint.h>

struct SubstrPos {
    int32_t start;
    int32_t end;
};

struct ParserResults {
    int32_t count;
    char matches[MAX_PARSER_MATCHES][MAX_PARSER_MATCH_SIZE];
};

// Find the first occurrence of pattern in buffer
char *parser_find(char *buffer, char *pattern);

// Find n occurences of pattern in buffer
struct ParserResults *parser_findall(char *buffer, char *pattern);

// Convert substring position to a string, free() after use
char *pos2str(char *buffer, struct SubstrPos *substr);

#endif /* CANIME_PARSER_H */
