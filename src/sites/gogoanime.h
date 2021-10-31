#ifndef CANIME_SITES_GOGOANIME_H
#define CANIME_SITES_GOGOANIME_H

struct SearchResults *gogoanime_search(char *query);
struct AnimeInfo *gogoanime_get_metadata(char *animeid);
void gogoanime_get_sources(struct AnimeInfo *anime);

#endif /* CANIME_SITES_GOGOANIME_H */
