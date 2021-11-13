#include <stdio.h>

#include "input.h"
#include "anime.h"
#include "common.h"
#include "colors.h"

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------
static void print_search_results(struct SearchResults *search_results)
{
    char *color;
    char *fmt = "%s[%s%d%s]%s %s%s%s\n";

    if (search_results->total > MAX_ANIME_SEARCH_RESULTS)
        search_results->total = MAX_ANIME_SEARCH_RESULTS;

    // print with colors
    for (int i = search_results->total - 1; i >= 0; i--) {
        color = (i % 2 == 0) ? C_BYEL : C_RESET;
        printf(fmt, C_BBLU, C_BCYN, i + 1, C_BBLU, C_RESET, color,
               search_results->results[i], C_RESET);
    }
}

static void print_options()
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

static bool handle_option_choice(char choice, struct AnimeInfo *anime)
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
        // ask_episode_sel() checks if episode is out of range
        anime->current_episode = ask_episode_sel(anime->total_episodes);
        return false;

    case 'q':
        return true;

    default:
        warn("Invalid Choice");
        return true;
    };

    if (anime->current_episode < 1 ||
        anime->current_episode > anime->total_episodes) {
        // canime shouldn't die here, we have to write history!
        warn("Episode out of range");
        return true;
    }

    return false;
}

// -----------------------------------------------------------------------------
// Non-Private Functions
// -----------------------------------------------------------------------------
char *ask_search_query()
{
    static char anime_name[MAX_ANIME_NAME_LEN];
    fputs("Enter Anime: ", stdout);
    fgets(anime_name, sizeof(anime_name), stdin);

    return anime_name;
}

int ask_anime_sel(struct SearchResults *search_results)
{
    int sel;

    print_search_results(search_results);
    printf("%sEnter Selection:%s ", C_BBLU, C_RESET);
    scanf("%2d", &sel);
    fflush(stdin);

    if (sel < 1 || sel > MAX_ANIME_SEARCH_RESULTS)
        die("Invalid Selection");

    return sel - 1;
}

int ask_episode_sel(int total_episodes)
{
    int sel;
    printf("%sChoose Episode %s[1-%d]%s: ", C_BBLU, C_BCYN, total_episodes,
           C_RESET);
    scanf("%2d", &sel);
    fflush(stdin);

    if (sel < 1 || sel > total_episodes)
        die("Invalid Episode");

    return sel;
}

bool ask_option_choice(struct AnimeInfo *anime)
{
    char choice;

    print_options();
    printf("%sEnter Choice%s: ", C_BBLU, C_RESET);
    scanf(" %c", &choice);

    return handle_option_choice(choice, anime);
}
