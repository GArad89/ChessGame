#include "CHMinimaxNode.h"

CHMinmaxNode* chMinmaxNodeCreate(CHGame* src,int depth,int player){
	if(src == NULL){ //NULL pointer received as an argument
		return NULL;
	}
	CHMinmaxNode* node	=	(CHMinmaxNode*) malloc(sizeof(CHMinmaxNode));
	if(node	==	NULL){ //memory allocation failed
		return NULL;
	}

	node->game	=	src;
	node->depth	=	depth;
	node->player=player;
	node->error_flag = 10;
	return node;
}

CHMinmaxNode* chMinmaxNodeCreateChild(CHMinmaxNode* src,int origin, int target){//we check that origin -> target is a valid move before
	if(src == NULL){ //NULL pointer received as an argument
		src->error_flag = min(src->error_flag,3);
		return NULL;
	}
	CHGame* game	=	chGameCopy(src->game);
	if(game	==	NULL){  //game copy failed
		src->error_flag = min(src->error_flag,0);
		return NULL;
	}
	if(chGameSetMove(game, origin,target) != CH_GAME_SUCCESS){ //set move failed
		src->error_flag = min(src->error_flag,2);
		return NULL;
	}

	CHMinmaxNode* node= chMinmaxNodeCreate(game,src->depth+1,src->player);

	if(node	==	NULL){ //node create failed
		src->error_flag = min(src->error_flag,0);
		chGameDestroy(game);
		return NULL;
	}
	return node;
}


void chMinmaxNodeDestroy(CHMinmaxNode* src){
	if(src != NULL){
		chGameDestroy(src->game);
		free(src);
	}
}

int chMinmaxNodeScoring(CHGame* src){
	if(src == NULL){
		return -1002;
	}
	int result	=	0;
	char temp;
	int i,mod;
	mod = 1;
	for(i	=	0; i	<	CH_GAME_N_ROWS*CH_GAME_N_COLUMNS; i++){
		temp=src->gameBoard[chGameRow(i)][chGameCol(i)];
		if(temp != CH_GAME_EMPTY_ENTRY){ //piece is not an empty tile
			if(chGameGetColor(src,i) == 1){ //piece is white
				mod = 1;
			}
			else{
				mod = -1;
				temp = temp -'A'+'a';
			}
		}
		else{
			mod = 0;
		}

		switch(temp){
			case CH_PLAYER_1_PAWN:
				result+=mod;
				break;
			case CH_PLAYER_1_BISHOP:
				result+=3*mod;
				break;
			case CH_PLAYER_1_KNIGHT:
				result+=3*mod;
				break;
			case CH_PLAYER_1_ROOK:
				result+=5*mod;
				break;
			case CH_PLAYER_1_QUEEN:
				result+=9*mod;
				break;
			case CH_PLAYER_1_KING:
				result+=100*mod;
				break;
		}
	}  // end of outer for  (rows)
	return result;
}
