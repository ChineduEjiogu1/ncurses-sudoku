#ifndef SOLVER_H
#define SOLVER_H

#include "sudoku.h"

int solve_grid(int grid[9][9]);
int is_valid_placement(int grid[9][9], int row, int col, int num);
int has_unique_solution(int grid[9][9]);
int count_solutions(int grid[9][9]);
int find_empty_cell(int grid[9][9], int *row, int *col);
int is_grid_valid(int grid[9][9]);
int is_grid_complete(int grid[9][9]);

#endif