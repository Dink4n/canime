#ifndef CANIME_UTILS_H
#define CANIME_UTILS_H

#include <stdbool.h>

#include "parser.h"
#include "anime.h"

#define MAX_TEXT_BUFFER_SIZE 256
#define JOIN_STR(...) join_str((char *[]){ __VA_ARGS__, NULL })

// Ask user for search query
char *ask_search_query();

// Ask user for anime selection
int ask_anime_sel();

// Ask user for episode selection
int ask_episode_sel(int total_episodes);

// Ask user for option choice
bool handle_option_choice(char choice, struct AnimeInfo *anime);

// Print anime search results
void print_search_results(struct ParserResults *search_results);

// Play the anime
void play_episode(struct AnimeInfo *metadata);

// Join strings. Be sure to free() it
char *join_str(char **str_list);

#endif /* CANIME_UTILS_H */
