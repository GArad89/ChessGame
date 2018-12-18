/* i was allowed to submit the project without a partner by Moshe Sulamy */

#include "CHMainAux.h"
#include <string.h>

/**
 * naming conventions:
 * - all modules (source and  header files) start with CH (as for chess) not including main.c.
 * - public function start with the module name as a prefix but with ch instead of CH.
 * for example: in CHGame.c there is the function chGameCreate
 * - private functions don't have the module name as a prefix.
 * - structures start with a CH prefix. all besides one (CHMoveList) use their module's name
 *
 * notes regarding the game implementation:
 * - any postion on the gameboard that is passed as input or output is passed as a value from 0 to  CH_GAME_N_ROWS*CH_GAME_N_COLUMNS
 * (0 to 63 with default values). the values start with the bottom left (1-A) and goes right first and then up. <1-B> is 1. <2,A> is 8.
 * - the "costly" functions are chGameSetMove and chGameIsValidMove. all threatened postions are updated during setmove.
 * IsValudMove checks that the current player's king isn't in check after moving.
 *
 * Summary of structures in the program:
 * CHGame - holds all the information needed in order to carry on with the game.
 * CHArrayList - holds the last x moves made in the game and the pieces eaten by those moves. x=6 by default (3 moves for each player)
 * CHMoveList - hold an int array that represents the possible moves of a piece and the actual size of the array.
 * CHThreatMap - contains the implementation of the game's threat map. the map holds the information whether position x is threatened by position y.
 * CHMinmaxNode - a min max node used for the min max algorithm.
 *
 * modules summary:
 * CHMainAux - holds all the functions that are used to run the different stages of the game in gui or console mode (setting,game,load ect).
 * also hold the saving and loading functions
 * CHParser - converting the user's text input into game commands if the game is in console mode
 * CHGUI - holds all SDL based GUI (besides the main window that is start in CHMainAux). handles converting the user's mouse input into game commands
 * CHGame - all game logic functions of the game itself that aren't in a structure's specifc module (like the threatmap or CHArrayList) are in this module.
 * CHArrayList - implements CHArrayList structure
 * CHMinimaxNode - implements minimax node structure.
 * CHThreatMap - implements the threat map.
 * CHMinimax - implements the min max algorithm (including the game's score)
 */

int main(int argc, char* argv[]){

//next 2 lines are used to move the output to the eclipse console
setvbuf(stdout, NULL, _IONBF, 0);
setvbuf(stderr, NULL, _IONBF, 0);

if(argc==2){
	if(strcmp(argv[1],"-g")==0){ //run in graphic mode
		CHMainAuxRunGameGUI();
		return 1;
	}
}
CHGame* game=NULL;
game=CHMainAuxInitialize(); //run in console mode
if(game != NULL){
	CHMainAuxRunGameConsole(game);
	chGameDestroy(game);
}
return 1;

}


