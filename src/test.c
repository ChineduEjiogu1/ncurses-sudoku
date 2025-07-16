#include <stdio.h>
#include <unistd.h>
#include "../include/sudoku.h"
#include "../include/generator.h"
#include "../include/solver.h"
#include "../include/game.h"
#include "../include/input.h" // optional, if testing input

#define SIZE 9

void print_grid(int grid[SIZE][SIZE])
{
    printf("-------------------------\n");
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (col % 3 == 0)
                printf("| ");
            if (grid[row][col] == 0)
                printf(". ");
            else
                printf("%d ", grid[row][col]);
        }
        printf("|\n");
        if ((row + 1) % 3 == 0)
            printf("-------------------------\n");
    }
}

int main()
{
    game_state_t game;

    printf("=== Sudoku Game Module Test ===\n");

    // Step 1: Init game
    init_game(&game, MEDIUM);
    printf("\nGenerated Puzzle:\n");
    print_grid(game.grid);

    printf("\nSolution:\n");
    print_grid(game.solution);

    // Step 2: Test deletion at editable cell
    int editable = 0;
    for (int r = 0; r < GRID_SIZE && !editable; r++)
    {
        for (int c = 0; c < GRID_SIZE && !editable; c++)
        {
            if (!game.given[r][c])
            {
                game.cursor_row = r;
                game.cursor_col = c;
                enter_number(&game, 5); // Try placing 5
                printf("\nPlaced 5 at (%d, %d)\n", r, c);
                delete_number(&game);
                printf("Deleted number at (%d, %d)\n", r, c);
                editable = 1;
            }
        }
    }

    // Step 3: Solve and check
    printf("\nSolving Puzzle...\n");
    solve_puzzle(&game);
    print_grid(game.grid);

    printf("Checking solution...\n");
    if (is_game_complete(&game))
    {
        printf("✅ Puzzle marked as complete.\n");
    }
    else
    {
        printf("❌ Puzzle is not complete.\n");
    }

    // Step 4: Reset game
    printf("\nResetting puzzle to original state...\n");
    reset_game(&game);
    print_grid(game.grid);

    return 0;
}

// #include <ncurses/ncurses.h>
// #include "../include/sudoku.h"
// #include "../include/generator.h"
// #include "../include/solver.h"
// #include "../include/game.h"
// #include "../include/input.h"
// #include "../include/display.h" // <- you need redraw_screen here

// int main() {
//     game_state_t game;

//     // Initialize ncurses
//     initscr();
//     raw();
//     noecho();
//     keypad(stdscr, TRUE);
//     curs_set(0); // hide blinking cursor

//     // Setup game
//     init_game(&game, MEDIUM);
//     redraw_screen(&game);

//     // Interactive loop
//     while (handle_input(&game)) {
//         redraw_screen(&game);
//     }

//     // Clean up ncurses
//     endwin();
//     return 0;
// }

