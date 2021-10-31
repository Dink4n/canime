#include <stdio.h>
#include <regex.h> /* This is POSIX regex.h */
#include <string.h>

#include "regex.h" /* This is the wrapper */
#include "../common.h"

struct RegexResults *regex_findall(char *buffer, char *pattern)
{
    regex_t regex;
    regoff_t offset = 0;
    regmatch_t pmatch[2];
    static struct RegexResults results = { 0 };

    if (regcomp(&regex, pattern, REG_NEWLINE | REG_EXTENDED))
        die("regcomp: Cannot compile regex");

    int i;
    unsigned int length;
    regmatch_t substr_pos = { 0 };
    for (i = 0; i < MAX_REGEX_MATCHES; i++) {
        if (regexec(&regex, buffer + offset, ARRAY_SIZE(pmatch), pmatch, 0))
            break;

        substr_pos.rm_so = pmatch[1].rm_so + offset;
        substr_pos.rm_eo = pmatch[1].rm_eo + offset;
        length = substr_pos.rm_eo - substr_pos.rm_so;

        strncpy(results.matches[i], buffer + substr_pos.rm_so, length);
        results.matches[i][length] = '\0';

        offset += pmatch[1].rm_eo;
    }

    results.count = i;

    regfree(&regex);
    return &results;
}

char *regex_find(char *buffer, char *pattern)
{
    /* Return the first match */
    return regex_findall(buffer, pattern)->matches[0];
}
