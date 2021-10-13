#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "sites.h"
#include "web_client.h"
#include "parser.h"
#include "common.h"
#include "utils.h"

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

static char *_get_lastline(char *str, unsigned int length)
{
    bool line_found = false;

    // Reverse string
    for (char *ptr = str + length - 1; ptr > str; ptr--) {
        if (*ptr != '\n') {
            line_found = true;
            continue;
        }

        if (line_found)
            return ptr + 1;
    }

    return NULL;
}

// -----------------------------------------------------------------------------
// GogoAnimeInfo
// -----------------------------------------------------------------------------
struct ParserResults *gogoanime_search(char *query)
{
    assert(query != NULL);

    struct ParserResults *results;

    web_client_seturl(web_client, "https://gogoanime.vc/search.html", NULL);
    web_client_setpayload(web_client, "keyword", query);

    web_client_perform(web_client);

    results = parser_findall(web_client_getdata(web_client),
                   "<p class=\"name\"><a href=\"/category/([^\"]*)\".*",
                   MAX_ANIME_SEARCH_RESULTS);

    return results;
}

struct AnimeInfo *gogoanime_get_metadata(char *anime_id)
{
    assert(anime_id != NULL);

    struct AnimeInfo *metadata = malloc(sizeof(struct AnimeInfo));
    char *last_episode_str;
    int last_episode_num;

    char *url = JOIN_STR("https://gogoanime.vc/category/", anime_id, NULL);

    web_client_seturl(web_client, url, NULL);
    web_client_perform(web_client);

    last_episode_str = parser_find(web_client->webpage.buffer,
            ".*<a href=\"#\" class=\"active\" ep_start = '0' ep_end = '([^']+)'>.*");

    last_episode_num = strtol(last_episode_str, NULL, 10);

    metadata->title = anime_id;
    metadata->current_episode = 0;
    metadata->total_episodes = last_episode_num;
    metadata->episode = metadata->episode_cache;

    free(last_episode_str);

    return metadata;
}

void gogoanime_get_sources(struct AnimeInfo *anime)
{
    assert(anime != NULL);

    char *embedded_video_link, *video_url, *highq_video, *video_url_end;
    char *embedded_video_url;
    unsigned int highq_video_len;

    char *url = JOIN_STR("https://gogoanime.vc/", anime->title, "-episode-", _int2str(anime->current_episode));

    web_client_seturl(web_client, url, NULL);
    web_client_perform(web_client);

    embedded_video_link = parser_find(
        web_client->webpage.buffer,
        "^[[:space:]]*<a href=\"#\" rel=\"100\" data-video=\"([^\"]*)\" >.*");

    embedded_video_url = xstrdup(JOIN_STR("https:", embedded_video_link));
    web_client_seturl(web_client, embedded_video_url, NULL);

    web_client_perform(web_client);
    video_url = parser_find(web_client->webpage.buffer,
                            "^[[:space:]]*sources:\\[\\{file: '([^']*)'.*");

    web_client_seturl(web_client, video_url, embedded_video_url);
    web_client_perform(web_client);
    highq_video = _get_lastline(web_client->webpage.buffer, web_client->webpage.size);

    // Maximum video resolution is 1080. There is 4 characters in 1080
    // only thing that will change is that we add the resolution to the video_url.
    // the load time increases dramatically and the a '.' character just before
    // the resolution. Plus the null terminator
    video_url = xrealloc(video_url, strlen(video_url) + 5 + 1);
    video_url_end = strrchr(video_url, '/') + 1;
    highq_video_len = strlen(highq_video);

    // Remove the pesky newline character at the end
    memcpy(video_url_end, highq_video, highq_video_len - 1);
    video_url_end[highq_video_len - 1] = '\0';

    anime->episode->url = video_url;
    anime->episode->referer = embedded_video_url;

    free(embedded_video_link);
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

struct AnimeInfo *nineanime_get_metadata(char *anime_id)
{
    assert(anime_id != NULL);

    struct AnimeInfo *metadata = malloc(sizeof(struct AnimeInfo));

    return metadata;
}

void nineanime_get_sources(struct AnimeInfo *anime)
{
}

// -----------------------------------------------------------------------------
