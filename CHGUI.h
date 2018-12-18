
#ifndef CHGUI_H_
#define CHGUI_H_

#include "CHGame.h"
#include "CHMainAux.h"
#include <SDL.h>
#include <SDL_video.h>
#include "CHMinimax.h"


#define SAVE_PATH "./saves/Save_"
#define IMAGE_PATH_START "./images/Options/"
#define PIECE_PATH_START "./images/ChessPieces/"
#define IMAGE_PATH_END ".bmp"
#define SAVE_SLOTS 5 //in order to increase the number of save slots to X an bmp image with the same number need to be added to path.
// for example: setting SAVE_SLOTS to 6, would require the file "./images/Options/6.bmp" in order to work.

/**
 * function summary:
 *	chGUIStartWindow - Creates the start window.
 *	chGUISettingWindow - Creates the settings window.
 *	chGuiGameWindow - Creates the game window.
 *	chGuiSaveLoadWindow - Creates the save / load window.
 */

/**
 * Creates the start window.
 * the player has the option to start a new game, load game or quit.
 *
 * @param window - the pointer to the pointer (for checking that window started correctly) to the SDL window that was created for the game.
 * @return
 * 0 - an error occured or the player pressed on quit.
 * 2 - the player pressed on new game
 * 5 - the player pressed on load game
 */
int chGUIStartWindow(SDL_Window** window);
/**
 * Creates the settings window.
 * the player has the option to choose the number of players.
 * in case of 1 player the player can also choose his color and game difficulty.
 *
 * @param window - the pointer to the SDL window that was created for the game.
 * @param src - a pointer to pointer of the game object
 * @return
 * 0 - an error occured.
 * 3 - the player pressed on start
 * 1 - the player pressed on back
 */
int chGUISettingWindow(SDL_Window* window,CHGame** src);
/**
 * Creates the game window.
 *
 * @param window - the pointer to the SDL window that was created for the game.
 * @param src - a pointer to pointer of the game object
 * @return
 * 0 - an error occured, game is done or player pressed the X button at the corner, or pressed on quit.
 * 1 - the player pressed on main menu
 */
int chGuiGameWindow(SDL_Window* window,CHGame** src);
/**
 * Creates the save / load window.
 *
 * @param isLoad - indicate whether the window is for loading or saving. 1- load 0- save
 * @param src - a pointer to pointer of the game object
 * @return
 * 0 - an error occurred.
 * 2 - the player pressed on back or the X on the upper right corner.
 * 1 - the player loaded/saved sucessfully
 */
int chGuiSaveLoadWindow(int isLoad,CHGame** src);



#endif /* CHGUI_H_ */
