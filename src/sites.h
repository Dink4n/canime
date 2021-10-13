#ifndef CANIME_SITES_H
#define CANIME_SITES_H

#include "provider.h"

// Gogoanime
struct ParserResults *gogoanime_search(char *query);
struct AnimeInfo *gogoanime_get_metadata(char *anime_id);
void gogoanime_get_sources(struct AnimeInfo *anime);

// 9anime
struct ParserResults *nineanime_search(char *query);
struct AnimeInfo *nineanime_get_metadata(char *anime_Id);
void nineanime_get_sources(struct AnimeInfo *anime);

#endif /* CANIME_SITES_H */
