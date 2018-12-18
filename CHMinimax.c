#include "CHMinimax.h"
#include <stdlib.h>
#include "CHGame.h"


/// private functions declarations ///

/**
 * the function choose between position values i and j the following way:
 * check who has the lowest column value. if a position with lower value is found, return it (i or j).
 * otherwise, check who has the lowest row value. return it.
 *
 * assumptions :
 * Positions are valid positions.
 *  i != j
 *
 * @param i,j - positions on the gameboard
 * @return
 * i or j
 */
int Choose(int i, int j);

///**
// * the inner recursive function of chMinimaxSuggestMove.
// * runs recursively on the children of node(at the start) and return the score value according to the minmax algorithm.
// *
// *@params
// *CHMinmaxNode* node- the parent node
// *CHMinmaxTree* tree- the tree of the node.
// * @return
// *int- the score value according to the minmax algorithm that is explained in the assignment.
// */
int SuggestMove_Inner(CHMinmaxNode* src, int origin, int target, int prunvalue, int maxdepth);




int chMinimaxSuggestMove(CHGame* src, unsigned int maxDepth,int* best_mov_1,int* best_mov_2){
	if((src	==	NULL)||(maxDepth	<=	0)||(best_mov_1	==	NULL)||(best_mov_2	==	NULL)){
		return 0;
	}
	int mod; //modify the score value according to the current player color
	if(src->currentPlayer == 1){
		mod= 1;
	}
	else{
		mod =-1;
	}

	int value = -1001;
	int maxvalue=-1001;
	*best_mov_1=-1;
	*best_mov_2=-1;
	CHMinmaxNode* node = chMinmaxNodeCreate(chGameCopy(src),0,src->currentPlayer);
	CHMoveList mov;
	if(node == NULL){
		return 0;
	}
	int i,j,k;
	int pos;

	//run all the positions on the gameboard. get all the possible moves from the player's pieces and create a child node for them.
	for(i=0; i<CH_GAME_N_COLUMNS;i++){
		for(k = 0; k < CH_GAME_N_ROWS; k++){
			pos = chGamePos(k,i);
			if(chGameGetColor(src, pos) == src->currentPlayer){
				mov= chGameGetMoves (src,pos);
				for(j=0; j< mov.listSize  ;j++){
					value=mod*SuggestMove_Inner(node,pos,mov.moveList[j],maxvalue*mod,maxDepth);
					if(value>maxvalue){
						maxvalue=value;
						*best_mov_1 = pos;
						*best_mov_2 = mov.moveList[j];
					}
					else{
						if(value == maxvalue){
							if(pos == *best_mov_1){ //we are still check the moves of the best move piece.
								if(j == Choose(*best_mov_2,mov.moveList[j])){ //node->mov.moveList[j] has a lower column or row
									*best_mov_2 = mov.moveList[j];
								}
							}
							else{
								if(pos == Choose(*best_mov_1,pos)){ //pos has a lower column or row
									*best_mov_1 = pos;
									*best_mov_2= mov.moveList[j];
								}
							}
						}
					}
				}
			}
		}
	}
	if(node->error_flag < 10){ //error occurred.in order to not flood the console with error massage, we print only one (who is more or less more important)
		switch(node->error_flag){
		case 0:
			printf("Error: memory allocation error occurred in chMinimaxSuggestMove\n");
			break;
		case 1:
			printf("Error: null pointer error occurred in chMinimaxSuggestMove\n");
			break;
		case 2:
			printf("Error: received invalid move while running chMinimaxSuggestMove\n");
			break;
		case 3:
			printf("Error: null pointer error occurred in chMinmaxNodeCreateChild\n");
			break;
		case 4:
			printf("Error: null pointer error occurred in chMinmaxNodeScoring\n");
			break;
		}
	}
	chMinmaxNodeDestroy(node);
	return 1;
}

int SuggestMove_Inner(CHMinmaxNode* src,int origin, int target, int prunvalue, int maxdepth){
	CHMinmaxNode* node = chMinmaxNodeCreateChild(src,origin,target); //create a child node with the wanted move
	if(node == NULL){
		src->error_flag = min(1,src->error_flag );
		return -1002; //-1002 signal error with getting a value.
	}
	CHMoveList mov;
	if(node->depth	>=	maxdepth){ //max depth reached. return score value.
		int score = chMinmaxNodeScoring(node->game);
		if(score == -1002){ //error in scoring function
			src->error_flag = min(4,src->error_flag );
		}
		chMinmaxNodeDestroy(node);
		return score;
	}

	int value;
	int valuebest;
	if(node->game->currentPlayer == 1){ //we want to maximize the current player's own score. white's best score is 1000 and black's best is -1000
		value= -1001;
	}
	else{
		value= 1001;
	}
	valuebest=value;
	int i,j,k,pos;

	//get all possible moves and create child nodes for them
	for(i=0; i<CH_GAME_N_ROWS*CH_GAME_N_COLUMNS;i++){
		for(k = 0;k < CH_GAME_N_ROWS; k++){
			pos = chGamePos(k,i);
			if(chGameGetColor(node->game,pos) == node->game->currentPlayer){
				mov= chGameGetMoves (node->game,pos);
				for(j=0; j< mov.listSize;j++){
					value=SuggestMove_Inner(node,pos,mov.moveList[j],valuebest,maxdepth);
					if(value == -1002){ //error with getting a score
						src->error_flag = min(src->error_flag,4);
					}
					else{ //no error occured in scoring
						if(node->game->currentPlayer == 1){ //looking for highest value

							valuebest = max(valuebest,value);

							if(valuebest > prunvalue){ //pruning - the current maximum of the child score
								//is bigger then the current minimum of the node's parent
								//(since the parent is for the black player turn, it is looking for a minimum)
								// no reason to keep running on this node.
								src->error_flag = min(src->error_flag,node->error_flag);
								chMinmaxNodeDestroy(node);
								return valuebest;
							}
						}
						else{ //current node's turn is for a black player (looking for minimum)

							valuebest = min(valuebest,value);

							if(valuebest < prunvalue){ //child current minscore is smaller than the node's parent current max score
								src->error_flag = min(src->error_flag,node->error_flag);
								chMinmaxNodeDestroy(node);
								return valuebest;
							}
						}
					} //end of the "no error occured" else
				} //end of the inner for (index j)
			}
		} //end of outer for (index k)
	}  //end of the outer for (index i)
	src->error_flag = min(src->error_flag,node->error_flag);
	chMinmaxNodeDestroy(node);

	return valuebest;
}

//return the position with the lowest column. if columns are equal returns the position with the lowest row.
int Choose(int i, int j){
	if(chGameCol(i) <= chGameCol(j)){
		if(chGameCol(i) < chGameCol(j)){
			return i;
		}
		if(chGameRow(i) < chGameRow(j)){
			return i;
		}
	}
	return j;
}
