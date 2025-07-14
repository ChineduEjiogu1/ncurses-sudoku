#include "sudoku.h"
#include "game.h"
#include "display.h"
#include "input.h"

int main(int argc, char *argv[])
{
    game_state_t game;

    // Initialize ncurses
    init_ncurses();

    // Initialize game with default difficulty
    init_game(&game, EASY);

    // Main game loop
    game_loop(&game);

    // Cleanup and exit
    cleanup_ncurses();
    return 0;
}

void init_ncurses(void)
{
    initscr();            // Initialize ncurses
    cbreak();             // Disable line buffering
    noecho();             // Don't echo pressed keys
    keypad(stdscr, TRUE); // Enable arrow keys
    curs_set(0);          // Hide cursor

    // Initialize colors if terminal supports them
    if (has_colors())
    {
        start_color();
        init_colors(); // Call your color initialization function
    }
}

void cleanup_ncurses(void)
{
    endwin(); // End ncurses mode
}

void game_loop(game_state_t *game)
{
    while (1)
    {
        // Clear screen and draw everything
        clear();
        draw_game(game);
        refresh();

        // Handle input (returns 0 when user wants to quit)
        if (!handle_input(game))
        {
            break;
        }
    }
}