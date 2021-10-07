#ifndef CANIME_PROVIDER_H
#define CANIME_PROVIDER_H

#include "web_client.h"
#include "parser.h"

#define ANIME_NAME_LEN 128
#define MAX_ANIME_SEARCH_RESULTS 10
#define MAX_EPISODE_CACHE_SIZE 2

struct Episode {
    char *url;
    char *referer;
};

struct Anime {
    char *name;
    unsigned int current_episode;
    unsigned int total_episodes;
    struct Episode *episodes;
    struct Episode episode_cache[MAX_EPISODE_CACHE_SIZE];
};

struct AnimeProvider {
    char *name;
    struct ParserResults *(*search)(char *query);
    struct Anime (*get_metadata)(char *anime_id);
    char *(*get_sources)(struct Anime *anime);
    void (*play)();
};

// Get the anime provider
struct AnimeProvider *get_anime_provider(char *name);

#endif /* CANIME_PROVIDER_H */
