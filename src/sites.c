#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "sites.h"
#include "common.h"
#include "utils.h"
#include "web_client.h"

#include "parser/regex.h"
#include "parser/json.h"

#define INT2STR_SIZE 12

extern struct WebClient *web_client;
extern struct WebPage *web_page;

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------
static char *_int2str(int number)
{
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
// GogoAnime
// -----------------------------------------------------------------------------
struct SearchResults *gogoanime_search(char *query)
{
    assert(query != NULL);

    struct RegexResults *regex_results;
    static struct SearchResults results;

    web_client_seturl(web_client, "https://gogoanime.pe/search.html", NULL);
    web_client_setpayload(web_client, "keyword", query);

    web_client_perform(web_client);

    regex_results = regex_findall(web_page->buffer,
                   "<p class=\"name\"><a href=\"/category/([^\"]*)\".*");

    results.total = regex_results->count;
    for (int i = 0; i < MAX_ANIME_SEARCH_RESULTS; i++) {
        memcpy(results.results[i], regex_results->matches[i],
               MAX_ANIME_NAME_LEN);
    }

    return &results;
}

struct AnimeInfo *gogoanime_get_metadata(char *anime_id)
{
    assert(anime_id != NULL);

    struct AnimeInfo *metadata = malloc(sizeof(struct AnimeInfo));
    char *last_episode_str;
    int last_episode_num;

    char *url = JOIN_STR("https://gogoanime.pe/category/", anime_id, NULL);

    web_client_seturl(web_client, url, NULL);
    web_client_perform(web_client);

    last_episode_str = regex_find(web_page->buffer,
            ".*<a href=\"#\" class=\"active\" ep_start = '0' ep_end = '([^']+)'>.*");

    last_episode_num = strtol(last_episode_str, NULL, 10);

    metadata->title = anime_id;
    metadata->current_episode = 0;
    metadata->total_episodes = last_episode_num;

    return metadata;
}

void gogoanime_get_sources(struct AnimeInfo *anime)
{
    assert(anime != NULL);

    char *video_url, *highq_video, *video_url_end, *embedded_video_url;
    unsigned int highq_video_len;

    char *url = JOIN_STR("https://gogoanime.pe/", anime->title, "-episode-", _int2str(anime->current_episode));

    web_client_seturl(web_client, url, NULL);
    web_client_perform(web_client);

    embedded_video_url = regex_find(web_page->buffer,
        "^[[:space:]]*<a href=\"#\" rel=\"100\" data-video=\"([^\"]*)\" >.*");

    embedded_video_url = JOIN_STR("https:", embedded_video_url);

    web_client_seturl(web_client, embedded_video_url, NULL);
    web_client_perform(web_client);
    video_url = regex_find(web_page->buffer,
                            "^[[:space:]]*sources:\\[\\{file: '([^']*)'.*");

    web_client_seturl(web_client, video_url, embedded_video_url);
    web_client_perform(web_client);
    highq_video = _get_lastline(web_page->buffer, web_page->size);

    // Copy referer and url to the result
    strncpy(anime->episode.referer, embedded_video_url, sizeof(anime->episode.referer) - 1);
    anime->episode.referer[sizeof(anime->episode.referer) - 1] = '\0';

    strncpy(anime->episode.url, video_url,
            sizeof(anime->episode.url) - 1);

    // Get the file name part from the video url
    video_url_end = strrchr(anime->episode.url, '/') + 1;
    highq_video_len = strlen(highq_video);

    // Replace the filename with highq_video
    // Also remove the pesky newline character at the end
    memcpy(video_url_end, highq_video, highq_video_len - 1);
    video_url_end[highq_video_len - 1] = '\0';
}

// -----------------------------------------------------------------------------
// AnimePahe
// -----------------------------------------------------------------------------
struct SearchResults *animepahe_search(char *query)
{
    assert(query != NULL);

    static struct SearchResults results;

    web_client_seturl(web_client, "https://animepahe.com/api?l=8&m=search", NULL);
    web_client_setpayload(web_client, "q", query);
    web_client_perform(web_client);

    return &results;
}

struct AnimeInfo *animepahe_get_metadata(char *anime_id)
{
    assert(anime_id != NULL);

    struct AnimeInfo *metadata = malloc(sizeof(struct AnimeInfo));

    return metadata;
}

void animepahe_get_sources(struct AnimeInfo *anime)
{
}

// -----------------------------------------------------------------------------
