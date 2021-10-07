#include <string.h>
#include <stdint.h>

#include "provider.h"
#include "common.h"
#include "sites.h"

struct AnimeProvider *get_anime_provider(char *name)
{
    static struct AnimeProvider providers[] = {
        { "gogoanime", gogoanime_search, gogoanime_get_metadata, gogoanime_get_sources },
        { "9anime", nineanime_search, nineanime_get_metadata, },
    };

    for (unsigned int i = 0; i < ARRAY_SIZE(providers); i++) {
        if (strcmp(name, providers[i].name) == 0) {
            return &providers[i];
        }
    }

    return NULL;
}
