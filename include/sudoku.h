#ifndef SUDOKU_H
#define SUDOKU_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#define GRID_SIZE 9
#define BOX_SIZE 3

typedef enum
{
    EASY = 0,
    MEDIUM,
    HARD,
    EXPERT
} difficulty_t;

typedef struct
{
    int grid[GRID_SIZE][GRID_SIZE];
    int solution[GRID_SIZE][GRID_SIZE];
    int given[GRID_SIZE][GRID_SIZE];
    int marks[GRID_SIZE][GRID_SIZE][GRID_SIZE];
    int show_marks;
    int cursor_row, cursor_col;
    difficulty_t difficulty;
    int moves;
    time_t start_time; // When game started
    time_t pause_time; // For pause functionality
    int is_paused;     // Pause state
    int is_completed;  // Game finished
} game_state_t;

#endif /* SUDOKU_H */