#ifndef CANIME_ANIME_H
#define CANIME_ANIME_H

#include "web_client.h"

#define MAX_ANIME_NAME_LEN            128
#define MAX_ANIME_SEARCH_RESULTS      10

struct Episode {
    char url[URL_BUF_SIZE];
    char referer[URL_BUF_SIZE];
};

struct AnimeInfo {
    char *title;
    unsigned int current_episode;
    unsigned int total_episodes;
    struct Episode episode;
};

struct SearchResults {
    unsigned int total;
    char results[MAX_ANIME_SEARCH_RESULTS][MAX_ANIME_NAME_LEN];

    // Addtional metadata
    void *metadata;
};

#endif /* CANIME_ANIME_H */
