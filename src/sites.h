#ifndef CANIME_SITES_H
#define CANIME_SITES_H

#include "provider.h"

// Gogoanime
struct ParserResults *gogoanime_search(char *query);
struct Anime gogoanime_get_metadata(char *anime_id);
char *gogoanime_get_sources(struct Anime *anime);
void gogoanime_play(struct Anime *anime);

// 9anime
struct ParserResults *nineanime_search(char *query);
struct Anime nineanime_get_metadata(char *anime_Id);
char *nineanime_get_sources(struct Anime *anime);
void nineanime_play(struct Anime *anime);

#endif /* CANIME_SITES_H */
