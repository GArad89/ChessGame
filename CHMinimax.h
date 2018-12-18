#ifndef CHMINIMAX_H_
#define CHMINIMAX_H_

#include "CHGame.h"
#include "CHMinimaxNode.h"


//**
// * initiate a min max algorithm run on currentGame up to maxDepth value. update the values at best_mov_1 (origin) and
// *  best_mov_2 (target) to the best move according to the algorithm.
// *
// *@params
// *CHGame* currentGame- the game that the algorithm runs on
// *unsigned int maxDepth- the maximum depth that the recursive inner function will reach.
// * int* best_mov_1 - the pointer to the position value of the piece to be moved
// * int* best_mov_2 - the pointer to the position value of that the piece will be moved to
// * @return
// *1 - success
// *0 - otherwise
// *
// */
int chMinimaxSuggestMove(CHGame* currentGame, unsigned int maxDepth,int* best_mov_1,int* best_mov_2);




#endif
