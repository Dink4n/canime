#include "base.h"
#include "animepahe.h"

#include "../parser/json.h"

static char *web_page;

// -----------------------------------------------------------------------------
// AnimePahe
// -----------------------------------------------------------------------------
struct SearchResults *animepahe_search(char *query)
{
    static struct SearchResults results;

    web_client_seturl("https://animepahe.com/api?l=8&m=search", NULL);
    web_client_setpayload("q", query);
    web_page = web_client_perform();

    return &results;
}

struct AnimeInfo *animepahe_get_metadata(char *animeid)
{
    struct AnimeInfo *metadata = xmalloc(sizeof(struct AnimeInfo));

    return metadata;
}

void animepahe_get_sources(struct AnimeInfo *anime)
{
}
