#ifndef CANIME_ANIME_H
#define CANIME_ANIME_H

#define ANIME_NAME_LEN 128
#define MAX_EPISODE_CACHE_SIZE 3

struct Episode {
    char *url;
    char *referer;
};

struct AnimeInfo {
    char *title;
    unsigned int current_episode;
    unsigned int total_episodes;
    struct Episode episode_cache[MAX_EPISODE_CACHE_SIZE];
    struct Episode *episode;
};

struct SearchResults {
    unsigned int total;
    char **results;

    // Addtional metadata
    void *meta;
};

#endif /* CANIME_ANIME_H */
