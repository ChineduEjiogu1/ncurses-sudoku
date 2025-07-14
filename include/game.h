#ifndef GAME_H
#define GAME_H

#include "sudoku.h"

void init_game(game_state_t *game, difficulty_t difficulty);
void check_solution(game_state_t *game);
void new_puzzle(game_state_t *game);
void solve_puzzle(game_state_t *game);
void enter_number(game_state_t *game, int num);
void delete_number(game_state_t *game);
void toggle_marks(game_state_t *game);
int is_valid_move(game_state_t *game, int row, int col, int num);
int is_game_complete(game_state_t *game);
int is_cell_given(game_state_t *game, int row, int col);
void reset_game(game_state_t *game);

// Timer functions
void start_timer(game_state_t *game);
void pause_timer(game_state_t *game);
void resume_timer(game_state_t *game);
int get_elapsed_time(game_state_t *game);

#endif /* GAME_H */