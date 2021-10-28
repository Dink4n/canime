#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "regex.h"

char *regex_find(char *buffer, char *pattern)
{
    regex_t regex;
    regmatch_t pmatch[2];
    unsigned int length;
    struct SubstrPos substr_pos;
    static char result[MAX_REGEX_MATCH_SIZE] = { 0 };

    if (regcomp(&regex, pattern, REG_NEWLINE | REG_EXTENDED))
        die("regcomp: Cannot compile regex");

    substr_pos.start = 0;
    substr_pos.end = 0;
    if (!regexec(&regex, buffer, ARRAY_SIZE(pmatch), pmatch, 0)) {
        substr_pos.start = pmatch[1].rm_so;
        substr_pos.end = pmatch[1].rm_eo;
    }

    length = substr_pos.end - substr_pos.start;
    strncpy(result, buffer + substr_pos.start, length);
    result[length] = '\0';

    regfree(&regex);
    return result;
}

struct RegexResults *regex_findall(char *buffer, char *pattern)
{
    regex_t regex;
    regoff_t offset = 0;
    regmatch_t pmatch[2];
    static struct RegexResults results = { 0 };

    if (regcomp(&regex, pattern, REG_NEWLINE | REG_EXTENDED))
        die("regcomp: Cannot compile regex");

    int i;
    struct SubstrPos substr_pos = { 0 };
    unsigned int length;

    for (i = 0; i < MAX_REGEX_MATCHES; i++) {
        if (regexec(&regex, buffer + offset, ARRAY_SIZE(pmatch), pmatch, 0))
            break;

        substr_pos.start = pmatch[1].rm_so + offset;
        substr_pos.end = pmatch[1].rm_eo + offset;
        length = substr_pos.end - substr_pos.start;

        strncpy(results.matches[i], buffer + substr_pos.start, length);
        results.matches[i][length] = '\0';

        offset += pmatch[1].rm_eo;
    }

    results.count = i;

    regfree(&regex);
    return &results;
}
