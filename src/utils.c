#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "utils.h"
#include "common.h"
#include "provider.h"

extern struct WebClient *web_client;

char *ask_search_query()
{
    static char anime_name[ANIME_NAME_LEN];
    printf("Enter Anime: ");
    fgets(anime_name, sizeof(anime_name), stdin);

    return anime_name;
}

int ask_anime_sel()
{
    int sel;
    printf("Enter Selection: ");
    scanf("%2d", &sel);

    if (sel >= MAX_ANIME_SEARCH_RESULTS || sel <= 0)
        die("Invalid Selection");

    return sel - 1;
}

int ask_episode_sel(int total_episodes)
{
    int sel;
    printf("Select Episode [1-%d]: ", total_episodes);
    scanf("%2d", &sel);

    if (sel <= 0 || sel >= total_episodes)
        die("Invalid Episode");

    return sel;
}

void print_search_results(struct ParserResults *search_results)
{
    int length;

    // pretty print the search results
    for (int i = search_results->count; i >= 0; i--) {
        length =
            search_results->matches[i].end - search_results->matches[i].start;

        char *buffer = web_client_getdata(web_client);
        printf("[%d] %.*s\n", i + 1, length,
               buffer + search_results->matches[i].start);
    }
}

void play(char *referer, char *link)
{
    wait(NULL);
    if (fork() == 0) {
        printf("link is %s\n", link);

        // Set the referer
        char header_fields[21 + URL_BUF_SIZE] = "--http-header-fields=";
        strncat(header_fields, referer,
                sizeof(header_fields) - strlen(header_fields) - 1);

        char *command[] = { "mpv", "--really-quiet", header_fields, link, NULL };

        setsid();
        execvp(command[0], command);
        free(link);
        die("canime: execvp %s failed %s", command[0], strerror(errno));
    }
}
