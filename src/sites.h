#ifndef CANIME_SITES_H
#define CANIME_SITES_H

#include "provider.h"

// Gogoanime
struct SearchResults *gogoanime_search(char *query);
struct AnimeInfo *gogoanime_get_metadata(char *anime_id);
void gogoanime_get_sources(struct AnimeInfo *anime);

// animepahe
struct SearchResults *animepahe_search(char *query);
struct AnimeInfo *animepahe_get_metadata(char *anime_Id);
void animepahe_get_sources(struct AnimeInfo *anime);

#endif /* CANIME_SITES_H */
