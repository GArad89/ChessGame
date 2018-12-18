#ifndef CHMAINAUX_H_
#define CHMAINAUX_H_
#include "CHParser.h"
#include "CHGame.h"
#include <SDL.h>
#include <SDL_video.h>
#include "CHGUI.h"

/**
 * functions summary:
 *  CHMainAuxInitialize - runs the setting stage for the console mode.
 *  CHMainAuxRunGameConsole - runs the game stage for the console mode.
 *  CHMainAuxRunGameGUI - runs the entire program (after being called by main) for the GUI mode.
 *  CHMainAuxSave - saves the game into a given file path
 *  CHMainAuxLoad - loads a game from a given file path
 *
 */
//auxiliary functions and constants used by the main function:
#define INIT_MSG "Specify game settings or type 'start' to begin a game with the current settings:\n"
#define GAME_MOD "Game mode is set to "
#define DIFF_SET "Difficulty level is set to "
#define COLOR_SET "User color is set to "
#define DEF_SET "All settings reset to default\n"
#define NXT_MOV  "Enter your move "
#define W_WON "Checkmate! white player wins the game\n"
#define B_WON "Checkmate! black player wins the game\n"
#define TIE "The game ends in a draw\n"
#define EXT "Exiting...\n"
#define START "Starting game...\n"

//setting errors
#define ERR_GAME_MOD "Wrong game mode\n"
#define ERR_DIFF  "Wrong difficulty level. The value should be between 1 to 5\n"
#define ERR_COLOR  "Wrong user color. The value should be 0 or 1\n"
#define ERR_LOAD "Error: File doesn't exist or cannot be opened\n"
#define ERR_SAVE "File cannot be created or modified\n"

//move errors
#define ERR_INVALID_POS "Invalid position on the board\n"
#define ERR_EMPTY_POS "The specified position does not contain a player piece\n"
#define ERR_INVALID_PIECE "The specified position does not contain your piece\n"
#define ERR_MOV "Illegal move\n"
#define ERR_MOV_CHK_1 "Illegal move: king is still threatened\n"
#define ERR_MOV_CHK_2  "Illegal move: king will be threatened\n"

#define ERR_EMPTY_HIS "Empty history, no move to undo\n"



#define ERR_INVALID_CMD "Error: invalid command\n"
#define ERR_UNDO "Error: cannot undo previous move!\n"
#define ERR_CHOICE_CRIT "choose a response:'try_again' or any other input in order to quit\n"
#define ERR_CHOICE "choose a response:'try_again', 'ignore' or any other input in order to quit\n"



/**
 * runs the setting stage for the console mode.
 * the player may also send the quit command in order to leave.
 *
 * @return
 * CHGame* - on success,pointer to the chess game structure.
 * NULL      - otherwise
 */
CHGame* CHMainAuxInitialize(void);


/**
 * runs the game stage for the console mode.
 * the player may also send the quit command in order to leave.
 *@params
 *src - the target game to be run.
 * @return
 * 0 - error occurred
 * 1    - otherwise
 */
int CHMainAuxRunGameConsole(CHGame* src);

/**
 * runs the entire program (after being called by main) for the GUI mode.
 *
 * @return
 * 0 - error occurred
 * 1    - otherwise
 */
int CHMainAuxRunGameGUI(void);

/**
 * saves the src game into the file path fout
 * @params:
 * src - the game
 * fout - the file path (including the file name) to be save into
 *
 * @return
 * 0 - error occurred
 * 1    - otherwise
 */
int CHMainAuxSave(CHGame* src, const char* fout);
/**
 * load the save from the file path fin into the src game
 * @params:
 * src - the pointer to the game
 * fout - the file path (including the file name) to be loaded from
 *
 * @return
 * 0 - error occurred
 * 1    - otherwise
 */
int CHMainAuxLoad(CHGame** src, const char* fin);



#endif
