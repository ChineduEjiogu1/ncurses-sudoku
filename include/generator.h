#ifndef GENERATOR_H
#define GENERATOR_H

#include "sudoku.h"

void generate_puzzle(int grid[9][9], int solution[9][9], int given[9][9], difficulty_t difficulty);
int get_cells_to_remove(difficulty_t difficulty);
int is_valid_placement(int grid[9][9], int row, int col, int num);
int solve_grid(int grid[9][9]);
int find_empty_cell(int grid[9][9], int *row, int *col);
void shuffle_array(int *array, int num);
int get_cells_to_remove(difficulty_t difficulty);

#endif