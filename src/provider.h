#ifndef CANIME_PROVIDER_H
#define CANIME_PROVIDER_H

#include "web_client.h"
#include "parser.h"
#include "anime.h"

#define MAX_ANIME_SEARCH_RESULTS 10

struct AnimeProvider {
    char *name;
    struct ParserResults *(*search)(char *query);
    struct AnimeInfo *(*get_metadata)(char *anime_id);
    void (*get_sources)(struct AnimeInfo *anime);
};

// Get the anime provider
struct AnimeProvider *get_anime_provider(char *name);

#endif /* CANIME_PROVIDER_H */
