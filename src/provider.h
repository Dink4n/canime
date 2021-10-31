#ifndef CANIME_PROVIDER_H
#define CANIME_PROVIDER_H

#include "anime.h"

struct AnimeProvider {
    char *name;
    struct SearchResults *(*search)(char *query);
    struct AnimeInfo *(*get_metadata)(char *anime_id);
    void (*get_sources)(struct AnimeInfo *anime);
};

// Get the anime provider
struct AnimeProvider *get_anime_provider(char *name);

#endif /* CANIME_PROVIDER_H */
