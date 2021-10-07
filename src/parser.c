#include <stdio.h>
#include <string.h>

#include "common.h"
#include "parser.h"

struct SubstrPos parser_find(char *buffer, char *pattern)
{
    regex_t regex;
    regmatch_t pmatch[2];
    struct SubstrPos substr_pos;

    if (regcomp(&regex, pattern, REG_NEWLINE | REG_EXTENDED))
        die("regcomp: Cannot compile regex");

    if (regexec(&regex, buffer, ARRAY_SIZE(pmatch), pmatch, 0)) {
        substr_pos.start = 0;
        substr_pos.end = 0;
    }

    substr_pos.start = pmatch[1].rm_so;
    substr_pos.end = pmatch[1].rm_eo;

    regfree(&regex);
    return substr_pos;
}

struct ParserResults *parser_findall(char *buffer, char *pattern, uint16_t n)
{
    regex_t regex;
    regoff_t offset = 0;
    regmatch_t pmatch[2];
    struct ParserResults *results = xmalloc(n * sizeof(struct SubstrPos));

    if (regcomp(&regex, pattern, REG_NEWLINE | REG_EXTENDED))
        die("regcomp: Cannot compile regex");

    int i;
    for (i = 0; i < n; i++) {
        if (regexec(&regex, buffer + offset, ARRAY_SIZE(pmatch), pmatch, 0))
            break;

        results->matches[i].start = pmatch[1].rm_so + offset;
        results->matches[i].end = pmatch[1].rm_eo + offset;

        offset += pmatch[1].rm_eo;
    }

    results->count = i - 1;

    regfree(&regex);
    return results;
}

char *pos2str(char *buffer, struct SubstrPos *substr)
{
    int length = substr->end - substr->start;
    char *result = xmalloc(length + 1);

    strncpy(result, buffer + substr->start, length);
    result[length] = '\0';

    return result;
}
