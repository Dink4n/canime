#ifndef CANIME_SITES_ANIMEPAHE_H
#define CANIME_SITES_ANIMEPAHE_H

struct SearchResults *animepahe_search(char *query);
struct AnimeInfo *animepahe_get_metadata(char *animeid);
void animepahe_get_sources(struct AnimeInfo *anime);

#endif /* CANIME_SITES_ANIMEPAHE_H */
