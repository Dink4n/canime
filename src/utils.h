#ifndef CANIME_UTILS_H
#define CANIME_UTILS_H

#define MAX_TEXT_BUFFER_SIZE 256
#define JOIN_STR(...) join_str((char *[]){ __VA_ARGS__, NULL })

struct AnimeInfo;

// Convert int to string
char *int2str(int number);

// Get pointer to last line of string
char *get_lastline(char *str);

// Join strings. Be sure to free() it
char *join_str(char **str_list);

// Play the anime
void play_episode(struct AnimeInfo *metadata);

#endif /* CANIME_UTILS_H */
