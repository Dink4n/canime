#include <stdio.h>
#include <string.h>

#include "base.h"
#include "gogoanime.h"
#include "../parser/regex.h"

static char *web_page;

// -----------------------------------------------------------------------------
// GogoAnime
// -----------------------------------------------------------------------------
struct SearchResults *gogoanime_search(char *query)
{
    struct RegexResults *regex_results;
    static struct SearchResults results;

    web_client_seturl("https://gogoanime.pe/search.html", NULL);
    web_client_setpayload("keyword", query);

    web_page = web_client_perform();

    regex_results = regex_findall(web_page,
                   "<p class=\"name\"><a href=\"/category/([^\"]*)\".*");

    results.total = regex_results->count;
    for (int i = 0; i < MAX_ANIME_SEARCH_RESULTS; i++) {
        memcpy(results.results[i], regex_results->matches[i],
               MAX_ANIME_NAME_LEN);
    }

    return &results;
}

struct AnimeInfo *gogoanime_get_metadata(char *animeid)
{
    int last_episode_num;
    char *last_episode_str;
    struct AnimeInfo *metadata = malloc(sizeof(struct AnimeInfo));

    char *url = JOIN_STR("https://gogoanime.pe/category/", animeid, NULL);

    web_client_seturl(url, NULL);
    web_page = web_client_perform();

    last_episode_str = regex_find(web_page,
            ".*<a href=\"#\" class=\"active\" ep_start = '0' ep_end = '([^']+)'>.*");

    last_episode_num = strtol(last_episode_str, NULL, 10);

    metadata->title = animeid;
    metadata->current_episode = 0;
    metadata->total_episodes = last_episode_num;

    return metadata;
}

void gogoanime_get_sources(struct AnimeInfo *anime)
{
    char *video_url, *highq_video, *video_url_end, *embedded_video_url;
    unsigned int highq_video_len;

    char *url = JOIN_STR("https://gogoanime.pe/", anime->title, "-episode-", int2str(anime->current_episode));

    web_client_seturl(url, NULL);
    web_page = web_client_perform();

    embedded_video_url = regex_find(web_page,
            "^[[:space:]]*<a href=\"#\" rel=\"100\" data-video=\"([^\"]*)\" >.*");

    /* The resulting URL doesn't have `https:` part */
    embedded_video_url =  JOIN_STR("https:", embedded_video_url);

    web_client_seturl(embedded_video_url, NULL);
    web_page = web_client_perform();
    video_url = regex_find(web_page,
                            "^[[:space:]]*sources:\\[\\{file: '([^']*)'.*");

    web_client_seturl(video_url, embedded_video_url);
    web_page = web_client_perform();
    highq_video = get_lastline(web_page);

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
