#ifndef DISPLAY_H
#define DISPLAY_H

#include "../include/sudoku.h"

// Color pair constants
#define COLOR_NORMAL 1
#define COLOR_GIVEN 2
#define COLOR_CURSOR 3
#define COLOR_INVALID 4
#define COLOR_COMPLETE 5

void draw_game(game_state_t *game);
void draw_grid(game_state_t *game);
void draw_title_info(game_state_t *game);
void draw_help_panel(void);
void draw_status_message(const char *message);
void highlight_current_cell(int row, int col);
void draw_cell(int row, int col, int value, int is_given, int is_cursor, game_state_t *game);
void draw_marks(int row, int col, int marks[9]);
void init_colors(void);
void redraw_screen(game_state_t *game);
int is_valid_cell_placement(game_state_t *game, int row, int col, int value);
int should_highlight_cell(game_state_t *game, int check_row, int check_col);
void draw_cell(int row, int col, int value, int is_given, int is_cursor, game_state_t *game);
void clear_status_line(void);
void draw_completion_message(game_state_t *game);
void format_time(int seconds, char *buffer, size_t buffer_size);

#endif