#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sites.h"
#include "web_client.h"
#include "parser.h"

#define INT2STR_SIZE 12

extern struct WebClient *web_client;

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------
static char *_int2str(int number)
{
    // NOTE: May need to increase buffers
    static char result[INT2STR_SIZE];
    snprintf(result, sizeof(result), "%d", number);

    return result;
}

// -----------------------------------------------------------------------------
// GogoAnime
// -----------------------------------------------------------------------------
struct ParserResults *gogoanime_search(char *query)
{
    assert(query != NULL);

    struct ParserResults *results;

    web_client_seturl(web_client, WEB_CLIENT_URL( "https://gogoanime.vc/search.html"));
    web_client_setpayload(web_client, "keyword", query);

    web_client_perform(web_client);

    results = parser_findall(web_client_getdata(web_client),
                   "<p class=\"name\"><a href=\"/category/([^\"]*)\".*",
                   MAX_ANIME_SEARCH_RESULTS);

    return results;
}

struct Anime gogoanime_get_metadata(char *anime_id)
{
    assert(anime_id != NULL);

    struct Anime metadata;
    struct SubstrPos last_episode_pos;
    int last_episode_num;

    char *url[] = { "https://gogoanime.vc/category/", anime_id, NULL };

    web_client_seturl(web_client, url);
    web_client_perform(web_client);

    last_episode_pos = parser_find(web_client_getdata(web_client),
            ".*<a href=\"#\" class=\"active\" ep_start = '0' ep_end = '([^']+)'>.*");

    last_episode_num = strtol(web_client_getdata(web_client) + last_episode_pos.start,
            NULL, 10);

    metadata.name = anime_id;
    metadata.current_episode = 0;
    metadata.total_episodes = last_episode_num;

    return metadata;
}

char *gogoanime_get_sources(struct Anime *anime)
{
    assert(anime != NULL);

    struct SubstrPos dpage_url_pos, link_pos;
    char *dpage_url;
    char *link;

    char *url[] = {
        "https://gogoanime.vc/", anime->name, "-episode-", _int2str(anime->current_episode), NULL
    };

    web_client_seturl(web_client, url);
    web_client_perform(web_client);

    dpage_url_pos = parser_find(web_client_getdata(web_client),
        "^[[:space:]]*<a href=\"#\" rel=\"100\" data-video=\"([^\"]*)\" >.*");
    dpage_url = pos2str(web_client_getdata(web_client), &dpage_url_pos);

    web_client_seturl(web_client, (char *[]){ "https:", dpage_url, NULL });
    web_client_perform(web_client);
    link_pos = parser_find(web_client_getdata(web_client),
                "^[[:space:]]*sources:\\[\\{file: '([^']*)'.*");
    link = pos2str(web_client_getdata(web_client), &link_pos);

    return link;
}

void gogoanime_play(struct Anime *anime)
{
}

// -----------------------------------------------------------------------------
// 9anime
// -----------------------------------------------------------------------------
struct ParserResults *nineanime_search(char *query)
{
    assert(query != NULL);

    struct ParserResults *results = NULL;

    puts("Using nine anime");
    printf("Query: %s", query);

    return results;
}

struct Anime nineanime_get_metadata(char *anime_id)
{
    assert(anime_id != NULL);

    struct Anime metadata = {0};

    return metadata;
}

char *nineanime_get_sources(struct Anime *anime)
{
}

void nineanime_play()
{
}
// -----------------------------------------------------------------------------
