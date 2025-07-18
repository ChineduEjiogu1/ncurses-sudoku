/**
 * Sudoku Game Main Entry Point
 *
 * This file contains the main game loop and handles the overall program flow.
 * It initializes ncurses, sets up the game state, handles user input, and
 * manages the display updates including smooth timer functionality.
 *
 * Key Features:
 * - Non-blocking input with timeout for smooth timer updates
 * - Efficient redrawing (grid-only for cursor movement, full for game changes)
 * - Real-time timer display without screen flickering
 * - Complete input handling for all game commands
 */

#include "../include/sudoku.h"
#include "../include/display.h"
#include "../include/input.h"
#include "../include/game.h"
#include "../include/generator.h"
#include <ncurses.h>

/**
 * Main program entry point
 * Initializes the game environment, runs the main game loop, and handles cleanup
 *
 * @return 0 on successful program completion
 */
int main(void)
{
    game_state_t game;

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(250); // 250ms timeout for smooth timer updates

    init_colors();
    init_game(&game, MEDIUM);
    start_timer(&game);

    draw_game(&game); // Initial draw

    int last_time = -1;
    int continue_game = 1;

    while (continue_game)
    {
        int ch = getch();

        if (ch == ERR)
        {
            // Timeout - update timer if it changed
            int current_time = get_elapsed_time(&game);
            if (current_time != last_time)
            {
                attron(COLOR_PAIR(9));
                mvprintw(6, 50, "Time: %02d:%02d", current_time / 60, current_time % 60);
                attroff(COLOR_PAIR(9));
                refresh();
                last_time = current_time;
            }
        }
        else
        {
            // Handle input
            switch (ch)
            {
            // Arrow key movement controls
            case KEY_UP:
                move_cursor_up(&game);
                draw_grid(&game);
                break;
            case KEY_DOWN:
                move_cursor_down(&game);
                draw_grid(&game);
                break;
            case KEY_LEFT:
                move_cursor_left(&game);
                draw_grid(&game);
                break;
            case KEY_RIGHT:
                move_cursor_right(&game);
                draw_grid(&game);
                break;

            // Number input (1-9)
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                enter_number(&game, ch - '0');
                draw_game(&game);
                break;

            // Game commands
            case 'c':
                check_solution(&game);
                draw_game(&game);
                break;
            case 'n':
                new_puzzle(&game);
                start_timer(&game);
                draw_game(&game);
                break;
            case 'm':
                toggle_marks(&game);
                draw_game(&game);
                break;
            case 'r':
                draw_game(&game);
                break;
            case 's':
                solve_puzzle(&game);
                draw_game(&game);
                break;
            case 'x':
                delete_number(&game);
                draw_game(&game);
                break;

            // Hint system
            case 'h':
            {
                int hint_row, hint_col, hint_value;
                if (get_hint(&game, &hint_row, &hint_col, &hint_value))
                {
                    show_hint_message(&game, hint_row, hint_col); // Remove hint_value
                    draw_game(&game);

                    char hint_msg[100];
                    snprintf(hint_msg, sizeof(hint_msg),
                             "Hint: Try placing %d at row %d, column %d",
                             hint_value, hint_row + 1, hint_col + 1);
                    draw_status_message(hint_msg);
                }
                else
                {
                    draw_game(&game);
                    draw_status_message("No hints available - puzzle complete!");
                }
            }
            break;

            // Exit game
            case 'q':
            case 27: // ESC key
                continue_game = 0;
                break;

            default:
                break;
            }
        }

        if (is_game_complete(&game) && game.completion_time == 0)
        {
            game.completion_time = time(NULL);
            draw_completion_message(&game);
        }
    }

    endwin();
    return 0;
}