/*
 * This is a wrapper around the POSIX regex.h
 */

#ifndef CANIME_PARSER_H
#define CANIME_PARSER_H

#define MAX_REGEX_MATCHES 16
#define MAX_REGEX_MATCH_SIZE 256

struct RegexResults {
    unsigned int count;
    char matches[MAX_REGEX_MATCHES][MAX_REGEX_MATCH_SIZE];
};

// Find the first occurrence of pattern in buffer
char *regex_find(char *buffer, char *pattern);

// Find MAX_REGEX_MATCHES occurences of pattern in buffer
struct RegexResults *regex_findall(char *buffer, char *pattern);

#endif /* CANIME_PARSER_H */
