#include <unistd.h>

#include "common.h"
#include "colors.h"
#include "web_client.h"
#include "provider.h"
#include "utils.h"

#define DEFAULT_PROVIDER "gogoanime"

// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------
struct WebClient *web_client;
struct WebPage *web_page;

// -----------------------------------------------------------------------------
// Local Variables
// -----------------------------------------------------------------------------
static char *provider = DEFAULT_PROVIDER;
static struct AnimeProvider *anime_provider = NULL;
static char *progname = NULL;
static char *query = NULL;
static struct AnimeInfo *anime = NULL;

// -----------------------------------------------------------------------------
// Function Declarations
// -----------------------------------------------------------------------------
void init();
void get_initial_input();
void run();
void open_episode();
void usage();

// -----------------------------------------------------------------------------
// Function Definitions
// -----------------------------------------------------------------------------
void init()
{
    // Get the anime provider to use
    anime_provider = get_anime_provider(provider);
    if (anime_provider == NULL)
        die("Invalid Provider: %s", provider);

    // Initialize the web_client
    web_client = web_client_init();
    web_page = &web_client->webpage;
}

void get_initial_input()
{
    int anime_sel_id;
    int episode_sel;
    char *anime_sel = NULL;
    struct SearchResults *search_results = NULL;

    // If user didn't give query as argument. Then, ask for query
    if (query == NULL)
        query = ask_search_query();

    // Search the provider using the given query
    search_results = anime_provider->search(query);
    print_search_results(search_results);

    // Get anime selection from user
    anime_sel_id = ask_anime_sel();

    // Duplicate it in the heap
    anime_sel = xstrdup(search_results->results[anime_sel_id]);
    anime = anime_provider->get_metadata(anime_sel);

    // Get episode selection from user
    episode_sel = 1;
    if (anime->total_episodes > 1)
        episode_sel = ask_episode_sel(anime->total_episodes);

    anime->current_episode = episode_sel;
}

void run()
{
    open_episode();

    char choice;
    while (1) {
        printf("%sCurrent playing %s episode %s%d/%d%s\n", C_BGRN, anime->title,
               C_BCYN, anime->current_episode, anime->total_episodes, C_RESET);

        print_options();

        printf("%sEnter Choice%s: ", C_BBLU, C_RESET);
        scanf(" %c", &choice);
        if (handle_option_choice(choice, anime)) {
            break;
        }

        open_episode();
    }

    // Cleanup
    free(anime);
    web_client_cleanup(web_client);
}

void open_episode()
{
    // Clear the screen
    // TODO: Make this portable. Maybe use ncurses or something?
    fputs("\x1B[2J\x1B[1;1H", stdout);

    printf("Getting data for episode %d\n\n", anime->current_episode);
    anime_provider->get_sources(anime);
    play_episode(anime);
}


void usage()
{
    die("USAGE: %s [OPTION...] QUERY\n"
        " -h     show this help text\n"
        " -p     set provider to scrap\n"
        " -d     download episode\n"
        " -H     continue where you left off",
        progname);
}

int main(int argc, char *argv[])
{
    progname = argv[0];

    if (argc < 2)
        goto main_code;

    int opt;
    while ((opt = getopt(argc, argv, "hHdp:")) != -1) {
        switch (opt) {
        case 'h':
            usage();
            break;

        case 'H':
            break;

        case 'd':
            break;

        case 'p':
            provider = optarg;
            break;

        default: /* '?' */
            usage();
            break;
        }
    }

main_code:

    if (optind < argc)
        query = argv[optind];

    init();
    get_initial_input();
    run();


    return 0;
}
