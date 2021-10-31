#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "utils.h"
#include "common.h"
#include "anime.h"

#define INT2STR_SIZE 12

char *int2str(int number)
{
    static char result[INT2STR_SIZE];
    snprintf(result, sizeof(result), "%d", number);

    return result;
}

char *get_lastline(char *str)
{
    bool line_found = false;
    size_t length = strlen(str);

    // Reverse string
    for (char *ptr = str + length - 1; ptr > str; ptr--) {
        if (*ptr != '\n') {
            line_found = true;
            continue;
        }

        if (line_found)
            return ptr + 1;
    }

    return NULL;
}

void play_episode(struct AnimeInfo *anime)
{
    if (fork() == 0) {
        setsid();

        // Set the referer
        char *args = "--http-header-fields=Referer: ";
        char header_fields[strlen(args) + URL_BUF_SIZE];
        snprintf(header_fields, sizeof(header_fields),
                 "%s%s", args, anime->episode.referer);

        char *command[] = { "mpv", "--really-quiet", header_fields,
                            anime->episode.url, NULL };

        // execute mpv
        execvp(command[0], command);

        // If it got here, it's an error
        die("canime: execvp %s failed %s", command[0], strerror(errno));
    }
}

char *join_str(char **str_list)
{
    static char result[MAX_TEXT_BUFFER_SIZE];
    memset(result, 0, MAX_TEXT_BUFFER_SIZE);

    for (char **ptr = str_list; *ptr != NULL; ptr++) {
        strncat(result, *ptr, MAX_TEXT_BUFFER_SIZE - strlen(result) - 1);
    }

    return result;
}
