#ifndef INPUT_H
#define INPUT_H

#include "sudoku.h"

int handle_input(game_state_t *game);

void move_cursor_up(game_state_t *game);
void move_cursor_down(game_state_t *game);
void move_cursor_left(game_state_t *game);
void move_cursor_right(game_state_t *game);

int process_number_input(game_state_t *game, int num);
void handle_mark_input(game_state_t *game, int num);

// Input validation functions
int is_valid_cursor_position(int row, int col);
int can_enter_number(game_state_t *game, int row, int col);

#endif