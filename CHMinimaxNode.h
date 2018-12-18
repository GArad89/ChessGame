#include <stdlib.h>
#include "CHGame.h"

/**
 * functions summary:
 *  chMinmaxNodeCreate - Creates a minmax node with the specified depth and gameboard.
 *  chMinmaxNodeCreateChild -Creates a a copy of the minimax node src and adds a game move according to origin and target
 *  chMinmaxNodeDestroy - Frees all memory resources associated with the source node.
 *  chMinmaxNodeScoring - returns the score value of the CHGame's gameboard.
 *
 */
typedef struct ch_minmax_node {
	CHGame* game;
	int player;  //the player that called the minmax run (the pc's color usually). 0 - black 1- white
	int depth;
	int value; //the min/max score
	int prune_value; //the max/min value so far. (for pruning)
	int error_flag; //an error flag that indicates an error from child.
	//10- no error 0- memory 1-suggestmove received null pointers 2-invalid move (from get move) 3- createchild received null pointers 4-scoring function received null pointer
	//in case of more than one error. flag will be the lowest value error (memory error before invalid move and so on)
} CHMinmaxNode;


/**
 *  Creates a a copy of the minimax node src and adds a game move according to origin and target
 *  @param src - the parent of the node is the the node of the previous move or the current game start if the depth is 1.
 *  origin - the piece original position on the gameboard that the the node represents
 *  target - the piece end position on the gameboard that the the node represents
 *  @return
 *  NULL, if an allocation error occurred.
 *  A pointer to a minmax node otherwise.
 */
CHMinmaxNode* chMinmaxNodeCreateChild(CHMinmaxNode* src,int origin,int target);

/**
 *  Creates a minmax node with the specified depth and gameboard.
 *  @param depth - the distance of the node from the root (the original gameboard).
 *  src - the gameboard move the node represents
 *  @return
 *  NULL, if an allocation error occurred.
 *  An instant of an mixmax node otherwise.
 */
CHMinmaxNode* chMinmaxNodeCreate(CHGame* src,int depth,int player);

/**
 * Frees all memory resources associated with the source node.
 * @param src - the source node
 */
void chMinmaxNodeDestroy(CHMinmaxNode* src);

/**
 * returns the score value of the CHGame's gameboard.
 *
 * @param src - the game
 * @returns- an integer score value according to the live pieces values unless the game ended
 * 1000 - white player victory
 * (-1000) - black player victory
 *  0 - tie
 */
int chMinmaxNodeScoring(CHGame* src);

