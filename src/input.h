#ifndef CANIME_INPUT_H
#define CANIME_INPUT_H

#include <stdbool.h>

struct AnimeInfo;
struct SearchResults;

// Ask user for search query
char *ask_search_query();

// Ask user for anime selection
int ask_anime_sel(struct SearchResults *search_results);

// Ask user for episode selection
int ask_episode_sel(int total_episodes);

// Ask user for option selection
// @returns true if program should quit
bool ask_option_choice(struct AnimeInfo *anime);

#endif /* CANIME_INPUT_H */
