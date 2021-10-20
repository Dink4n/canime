#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "common.h"
#include "colors.h"
#include "provider.h"

extern struct WebClient *web_client;
extern struct WebPage *web_page;

char *ask_search_query()
{
    static char anime_name[MAX_ANIME_NAME_LEN];
    fputs("Enter Anime: ", stdout);
    fgets(anime_name, sizeof(anime_name), stdin);

    return anime_name;
}

int ask_anime_sel()
{
    int sel;
    printf("%sEnter Selection:%s ", C_BBLU, C_RESET);
    scanf("%2d", &sel);
    getchar();

    if (sel >= MAX_ANIME_SEARCH_RESULTS || sel <= 0)
        die("Invalid Selection");

    return sel - 1;
}

int ask_episode_sel(int total_episodes)
{
    int sel;
    printf("%sChoose Episode %s[1-%d]%s: ", C_BBLU, C_BCYN, total_episodes,
           C_RESET);
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
        anime->current_episode = ask_episode_sel(anime->total_episodes);
        break;

    case 'q':
        return true;

    default:
        printf("%sInvalid choice%s\n", C_BRED, C_RESET);
        return true;
    };

     return false;
}

void print_search_results(struct SearchResults *search_results)
{
    // Color print
    char *color;
    char *fmt = "%s[%s%d%s]%s %s%s%s\n";
    for (int i = search_results->total - 1; i >= 0; i--) {
        color = (i % 2 == 0) ? C_BYEL : C_RESET;
        printf(fmt, C_BBLU, C_BCYN, i + 1, C_BBLU, C_RESET, color,
               search_results->results[i], C_RESET);
    }
}

void print_options()
{
    char *fmt = "%s[%s%c%s]%s %s%s%s\n";

    printf(fmt, C_BBLU, C_BCYN, 'n', C_BBLU, C_RESET,
           C_BYEL, "next episode", C_RESET);
    printf(fmt, C_BBLU, C_BCYN, 'p', C_BBLU, C_RESET,
           C_BMAG, "previous episode", C_RESET);
    printf(fmt, C_BBLU, C_BCYN, 's', C_BBLU, C_RESET,
           C_BYEL, "select episode", C_RESET);
    printf(fmt, C_BBLU, C_BCYN, 'r', C_BBLU, C_RESET,
           C_BMAG, "replay episode", C_RESET);
    printf(fmt, C_BBLU, C_BCYN, 'q', C_BBLU, C_RESET,
           C_BRED, "quit", C_RESET);
}

void play_episode(struct AnimeInfo *metadata)
{
    if (fork() == 0) {
        // Set the referer
        char *args = "--http-header-fields=Referer: ";
        char header_fields[strlen(args) + URL_BUF_SIZE];
        snprintf(header_fields, sizeof(header_fields),
                 "%s%s", args, metadata->episode->referer);

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
