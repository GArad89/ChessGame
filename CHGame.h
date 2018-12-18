#ifndef SPFIARGAME_H_
#define SPFIARGAME_H_
#include <stdbool.h>
#include "CHArrayList.h"
#include <stdio.h>




/**
 * CHGame Summary:
 *
 * contains the logic implementation of a chess game. all functions related to the logic of the game itself that don't fit the dedicated
 *  modules like CHMinMax are implemented here.
 * chGameCreate           - Creates a new empty game board
 * chGameCopy             - Copies a game board
 * chGameDestroy          - Frees all memory resources associated with a game
 * chGameSetMove          - for a given move: Sets a move on a game board
 * chGameIsValidMove      - for a given move: Checks if a move is valid
 * chGameUndoPrevMove     - Undoes previous move made by the last player
 * chGamePrintBoard       - Prints the current board
 * chGameCheckWinner      - checks if the game ended and who won (including the case of a tie)
 * chGameGetColor         - for a given position: returns the color of the piece in a given position or if the position is empty.
 * chGameGetMoves         - for a given position: returns the list of possible moves for a piece in a given position
 * chGameCol              - for a given position: returns the column value.
 * chGameRow              - for a given position: returns the row value.
 * chGamePos              - for a given column and row: returns the position value.
 * chGameSettingsPrint    - print the game setting either to a file or stdout.
 * chGameStarting         - sets the gameboard to the starting position of a chess game
 * chGameUpdateThreat     - for a given position: update the threatened position by the given positions
 * chGameIsValidPos       - for a given position: returns true if the position is a valid board position. return false otherwise.
 * chGameIsHumanTurn      - returns true if the current player is a human. return false otherwise.
 *
 */

//basic min max macros
#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))

//Definitions
#define CH_GAME_N_ROWS 8
#define CH_GAME_N_COLUMNS 8
#define CH_HISTORY_SIZE 3 //the game will remember CH_HISTORY_SIZE*2 turns. (or CH_HISTORY_SIZE moves per player)
//player 1 is the white player (not necessarily the human player).
//player 1 symbols can be changed here but player 2 symbols will be hard coded to be the capital letters of player 1 symbols.
//(if player 1 pawn is 'x' then player 2 is x+'A'-'a')
#define CH_PLAYER_1_PAWN 'm'
#define CH_PLAYER_1_BISHOP 'b'
#define CH_PLAYER_1_ROOK 'r'
#define CH_PLAYER_1_KNIGHT 'n'
#define CH_PLAYER_1_QUEEN 'q'
#define CH_PLAYER_1_KING 'k'

#define CH_GAME_EMPTY_ENTRY '_'

//error and null pointer error massages are here since CHGame.h is included by most of the other modules.
#define ERR_MEM "Error: malloc has failed in the following function- "
#define ERR_NULL "Error: a null pointer recieved as an argument for the following function- "
#define ERR_POS_FUNC "Error: an invalid position value recieved for the following function-"
//the structure that holds all the chess game information
typedef struct ch_game_t {
	char gameBoard[CH_GAME_N_ROWS][CH_GAME_N_COLUMNS];
	int humanplayer;  //indicates the number of humans playing.
	int humancolor; // 1- white 0- black
	int currentPlayer;  //1-white 2-black
	int threatMap[(int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1];
	int diff; //Difficulty level
	int p1_king,p2_king; //both kings positions are kept here (in addition for the gameboard array) in order to check for checkmates
	bool isCheck; //indicator that the current player is checked.
	CHArrayList* move_list;
} CHGame;

//structure that holds a set of moves
typedef struct ch_game_move_list{
	int moveList[CH_GAME_N_ROWS*CH_GAME_N_COLUMNS];
	int listSize;
} CHMoveList;

/**
 * Type used for returning error codes from game functions
 */
typedef enum ch_game_message_t {
	CH_GAME_INVALID_MOVE,
	CH_GAME_INVALID_ARGUMENT,
	CH_GAME_NO_HISTORY,
	CH_GAME_SUCCESS,
	CH_GAME_INVALID_PIECE,
	CH_GAME_STILL_CHECK,
	CH_GAME_WILL_CHECK

} CH_GAME_MESSAGE;

/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @historySize - The total number of moves to undo,
 *                a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
CHGame* chGameCreate(int historySize);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
CHGame* chGameCopy(CHGame* src);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void chGameDestroy(CHGame* src);

/**
 * Sets the next move in a given game by specifying a starting position and target position. The
 * positions are in the range [0,SP_CH_GAME_N_COLUMNS*CH_GAME_N_ROWS] (0-63 with default values).
 * assumptions: either a check is being that the move is valid before calling this function, or the function is called by undo move.
 *
 * @param src - The target game
 * @param origin - the piece starting position before moving. values: [0,SP_CH_GAME_N_COLUMNS*CH_GAME_N_ROWS]
 * @param target - the piece postion after moving. values: [0,SP_CH_GAME_N_COLUMNS*CH_GAME_N_ROWS]
 * @return
 * CH_GAME_INVALID_ARGUMENT - if src is NULL or col is out-of-range
 * CH_GAME_SUCCESS - otherwise
 */
CH_GAME_MESSAGE chGameSetMove(CHGame* src, int origin,int target);

/**
 * Checks if a the move is a valid chess move. returns the fitting error massage in the case the move is not valid.
 *
 * @param src - The source game
 * @param origin - the piece starting position before moving. values: [0,SP_CH_GAME_N_COLUMNS*CH_GAME_N_ROWS]
 * @param target - the piece postion after moving. values: [0,SP_CH_GAME_N_COLUMNS*CH_GAME_N_ROWS]
 * @return
 * CH_GAME_INVALID_MOVE  - the move is illegal for the piece
 * CH_GAME_INVALID_ARGUMENT - bad input
 * CH_GAME_SUCCESS  - the move is valid
 * CH_GAME_INVALID_PIECE  - the piece in origin doesn't belong to the current player
 * CH_GAME_STILL_CHECK - the player's king is in check and the move doesn't defend the king.
 * CH_GAME_WILL_CHECK - the move will cause the player's king to be in check.
 */
CH_GAME_MESSAGE chGameIsValidMove(CHGame* src,int origin,int target);

/**
 * Returns the piece that was previously moved to it's original position, returns the piece that was eaten during the move
 * , update the threat map accordingly and changes the current player's turn.
 * If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * CH_GAME_INVALID_ARGUMENT - if src == NULL
 * CH_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * CH_GAME_SUCCESS           - on success. the game returns to the exact situation it was before the last move.
 */
CH_GAME_MESSAGE chGameUndoPrevMove(CHGame* src);

/**
 * On success, the function prints the board game to fout. if fout == NULL, the function prints to stdout.
 * If an error occurs, then the function does nothing.
 *
 * @param src - the target game
 * @return
 * CH_GAME_INVALID_ARGUMENT - if src==NULL
 * CH_GAME_SUCCESS - otherwise
 *
 */
CH_GAME_MESSAGE chGamePrintBoard(CHGame* src, FILE* fout);



/**
* Checks if there's a winner in the specified game status. The function returns either
* 'k' (for player 1 king) or 'K' (for player 2 king) in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value CH_GAME_EMPTY_ENTRY is returned. in any other case the 'N' character
* is returned.
* @param src - the source game
* @return
* 'k' - if player 1 won
* 'K' - if player 2 won
* CH_GAME_EMPTY_ENTRY - If the game is over and there's a tie
* 'N' - otherwise
*/
char chGameCheckWinner(CHGame* src);
/**
 * On success, the function returns the corresponding player value of the piece in the origin position.
 *
 * @param src - the target game
 * @param origin - the position to be checked
 * @return
 * 1 - the piece is white
 * 0 - the piece is black
 * 2 - the position is empty
 *
 */
int chGameGetColor(CHGame* src,int origin);
/**
 * On success, the function returns the valid moves of the piece in the origin position.
 *
 * @param src - the target game
 * @param origin - the position to be checked
 * @return
 * CHMoveList - the valid positions of the piece
 * NULL - otherwise (src == NULL or not a valid position)
 *
 */
CHMoveList chGameGetMoves(CHGame* src,int origin);


/**
 * returns the column of the given position value i.
 * @param i - the position to be checked
 * @return
 * int - the column value of i.
 *
 */
int chGameCol(int i);
/**
 * returns the row of the given position value i.
 * @param i - the position to be checked
 * @return
 * int - the row value of i.
 *
 */
int chGameRow(int i);
/**
 * returns the position value of the given column and row.
 * @param col - the column value
 * @param row - the row value
 * @return
 * int - the position value.
 *
 */
int chGamePos(int col,int row);

///**
// * prints the game's setting into fout if fout!= NULL. prints into stdout otherwise
// * @param src - the target game
// * @param fout - the target file
// * @return
// * 1 - success
// * 0 - failure
// *
// */
int chGameSettingsPrint(CHGame* src, FILE* fout);
/**
 * setup all the pices and threatmap to the starting position. return 1 on success. return 0 otherwise.
 * @param src - the target game
 * @return
 * 1 - success
 * 0 - failure
 *
 */
int chGameStarting(CHGame* src);
/**
 * update all the positioned threatened by the newly moved piece in target.
 * @param src - the target game
 * @param target - the position to be checked
 * @return
 * 1 - success
 * 0 - failure
 *
 */
int chGameUpdateThreat(CHGame* src, int target);
/**
 * returns true if pos is a valid gameboard position for the given  CH_GAME_N_ROWS ,CH_GAME_N_COLUMNS values.
 * return false otherwise.
 * @param pos - the position value
 * @return
 * true - pos is a valid postion
 * false - otherwise
 *
 */
bool chGameIsValidPos(int pos);
/**
 * returns true if the current turn in src belongs to a human player.
 * return false otherwise.
 * @param src - the target game
 * @return
 * true - the src->currentplayer value belongs to a human player
 * false - otherwise
 *
 */
bool chGameIsHumanTurn(CHGame* src);



#endif
