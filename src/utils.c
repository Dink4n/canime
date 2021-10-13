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
    fputs("Enter Anime: ", stdout);
    fgets(anime_name, sizeof(anime_name), stdin);

    return anime_name;
}

int ask_anime_sel()
{
    int sel;
    fputs("Enter Selection: ", stdout);
    scanf("%2d", &sel);
    getchar();

    if (sel >= MAX_ANIME_SEARCH_RESULTS || sel <= 0)
        die("Invalid Selection");

    return sel - 1;
}

int ask_episode_sel(int total_episodes)
{
    int sel;
    printf("Select Episode [1-%d]: ", total_episodes);
    scanf("%2d", &sel);
    getchar();

    if (sel <= 0 || sel >= total_episodes)
        die("Invalid Episode");

    return sel;
}

bool handle_option_choice(char choice, struct AnimeInfo *anime)
{
    switch (choice) {
    case 'n':
        anime->current_episode++;
        break;

    case 'p':
        anime->current_episode--;
        break;

    case 'r':
        // Do nothing just play it back again
        break;

    case 's':
        fputs("\x1B[2J\x1B[1;1H", stdout);
        anime->current_episode = ask_episode_sel(anime->total_episodes);
        break;

    case 'q':
        return true;

    default:
        puts("Invalid choice");
        return true;
    };

     return false;
}

void print_search_results(struct ParserResults *search_results)
{
    int length;

    // pretty print the search results
    for (int i = search_results->count - 1; i >= 0; i--) {
        length =
            search_results->matches[i].end - search_results->matches[i].start;

        printf("[%d] %.*s\n", i + 1, length,
               web_client->webpage.buffer + search_results->matches[i].start);
    }
}

void play_episode(struct AnimeInfo *metadata)
{
    if (fork() == 0) {
        // Set the referer
        char header_fields[32 + URL_BUF_SIZE];
        snprintf(header_fields, sizeof(header_fields),
                 "--http-header-fields=Referer: %s", metadata->episode->referer);

        char *command[] = { "mpv", "--really-quiet", header_fields,
                            metadata->episode->url, NULL };

        setsid();
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
