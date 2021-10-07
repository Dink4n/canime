#ifndef CANIME_UTILS_H
#define CANIME_UTILS_H

#include "parser.h"

// Ask user for search query
char *ask_search_query();

// Ask user for anime selection
int ask_anime_sel();

// Ask user for episode selection
int ask_episode_sel(int total_episodes);

// Print anime search results
void print_search_results(struct ParserResults *search_results);

// Play the video url
void play(char *referer, char *link);

#endif /* CANIME_UTILS_H */
