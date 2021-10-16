#include <unistd.h>

#include "common.h"
#include "web_client.h"
#include "provider.h"
#include "utils.h"

#define DEFAULT_PROVIDER "gogoanime"

// Global Variables
struct WebClient *web_client;

// Local Variables
static char *provider = DEFAULT_PROVIDER;
static struct AnimeProvider *anime_provider = NULL;
static char *progname = NULL;
static char *query = NULL;

void open_episode(struct AnimeInfo *anime)
{
    // Clear the screen
    fputs("\x1B[2J\x1B[1;1H", stdout);

    printf("Getting data for episode %d\n\n", anime->current_episode);
    anime_provider->get_sources(anime);
    play_episode(anime);
}

void run()
{
    int anime_sel_id;
    char *anime_sel;
    int episode_sel;
    struct AnimeInfo *anime;
    struct ParserResults *search_results;

    // Get the anime provider to use
    anime_provider = get_anime_provider(provider);
    if (anime_provider == NULL)
        die("Invalid Provider: %s", provider);

    // Initialize the web_client
    web_client = web_client_init();

    // If user didn't give query as argument. Then, ask for query
    if (query == NULL)
        query = ask_search_query();

    // Search the provider using the given query
    search_results = anime_provider->search(query);
    print_search_results(search_results);

    // Get anime selection from user
    anime_sel_id = ask_anime_sel();

    // convert SubstrPos to string
    anime_sel = pos2str(web_client->webpage.buffer, &search_results->matches[anime_sel_id]);
    anime = anime_provider->get_metadata(anime_sel);

    // Get episode selection from user
    episode_sel = ask_episode_sel(anime->total_episodes);

    anime->current_episode = episode_sel;
    open_episode(anime);

    char choice;
    while (1) {
        printf("Current playing %s episode %d/%d\n", anime->title,
               anime->current_episode, anime->total_episodes);
        puts("[n] next episode");
        puts("[p] previous episode");
        puts("[r] replay episode");
        puts("[s] select episode");
        puts("[q] quit");

        fputs("Enter Choice: ", stdout);
        scanf(" %c", &choice);
        if (handle_option_choice(choice, anime)) {
            break;
        }

        open_episode(anime);
    }

    // Cleanup
    free(anime);
    free(anime_sel);
    free(search_results);
    web_client_cleanup(web_client);
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
    run();

    return 0;
}
