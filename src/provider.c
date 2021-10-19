#include <string.h>
#include <stdint.h>

#include "provider.h"
#include "common.h"
#include "sites.h"

struct AnimeProvider *get_anime_provider(char *name)
{
    static struct AnimeProvider providers[] = {
        { "gogoanime", gogoanime_search, gogoanime_get_metadata, gogoanime_get_sources },
        { "animepahe", animepahe_search, animepahe_get_metadata, animepahe_get_sources },
    };

    for (unsigned int i = 0; i < ARRAY_SIZE(providers); i++) {
        if (strcmp(name, providers[i].name) == 0) {
            return &providers[i];
        }
    }

    return NULL;
}
