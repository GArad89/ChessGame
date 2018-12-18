#include "CHGame.h"
#include "CHArrayList.h"
#include "CHThreatMap.h"
#include <stdlib.h>
#include <stdbool.h>


// private functions declarations //

/**
 * sub-function of chGameUpdateThreat.
 * updates the positions that are now threatened by the pawn that moved to the position "target".
 *
 * @param
 * src - the target game
 * target - the piece target position
 * mod - the color of the piece (black or white)
 */
void UpdateThreatPawn(CHGame* src, int target,int mod);
/**
 * sub-function of chGameUpdateThreat.
 * updates the positions that are now threatened by the line piece (queen,bishop or rook) that moved to the position "target".
 *
 * @param
 * src - the target game
 * target - the piece target position
 * maxvalue - the number of steps that are needed to take in the line direction in order to reach the edge of the board
 * dir1 - the horizontal direction of the line 1- right (-1) - left
 * dir2 - the vertical direction of the line 1- up  (-1) - down
 * *if dir1=1 and dir2=1 then we are updating the diagonal line in the right-up direction
 */
void UpdateThreatLine(CHGame* src, int target, int maxvalue, int dir1,int dir2);
/**
 * sub-function of chGameUpdateThreat.
 * updates the positions that are now threatened by the knight that moved to the position "target".
 *
 * @param
 * src - the target game
 * target - the piece target position
 */
void UpdateThreatKnight(CHGame* src, int target);
/**
 * sub-function of chGameUpdateThreat.
 * updates the positions that are now threatened by the king that moved to the position "target".
 *
 * @param
 * src - the target game
 * target - the piece target position
 */
void UpdateThreatKing(CHGame* src, int target);

/**
 * On success, the function returns true if the game is in check and the move from origin to target defends the king (no longer in check).
 * return false otherwise
 * assumptions: the game is in check.
 *
 * @param src - the target game
 * @param origin - the piece starting position
 * @param target - the piece target position
 * @return
 * true - the move defends the king
 * false - the move doesn't defend the king or src == NULL or either origin or target are not a valid position
 *
 */
bool IsDefendCheck(CHGame* src, int origin,int target);

/**
 * On success, the function returns true if  the move from origin to target causes the player's own king to be in check.
 * return false otherwise
 *
 * @param src - the target game
 * @param origin - the piece starting position
 * @param target - the piece target position
 * @return
 * true - the move causes the player's king to be in check.
 * false - otherwise or src == NULL or either origin or target are not a valid position
 *
 */
bool IsSelfCheck(CHGame* src, int origin,int target);

/**
 * On success, the function returns the corresponding player value of the piece in the origin position ONLY if it is a knight.
 * returns (-1) for bad input. returns 2 otherwise.
 * @param src - the target game
 * @param origin - the position to be checked
 * @return
 * 1 - the piece is a white knight
 * 0 - the piece is a black knight
 * (-1) - bad input (src == NULL) or invalid position.
 * 2 - otherwise
 *
 */
int IsKnight(CHGame* src, int origin);

/**
 * On success, the function returns the corresponding player value of the piece in the origin position ONLY if it is a pawn.
 * returns (-1) for bad input. returns 2 otherwise.
 * @param src - the target game
 * @param origin - the position to be checked
 * @return
 * 1 - the piece is a white pawn
 * 0 - the piece is a black pawn
 * (-1) - bad input (src == NULL) or invalid position.
 * 2 - otherwise
 *
 */
int IsPawn(CHGame* src, int origin);

/**
 * if i and target are on the same line, returns a value according to the direction from i to j
 * returns 0 otherwise
 * @param i,j - the position values to be checked.
 * @param row - the row value
 * @return
 * 0 - i and j are not on the same line
 * 1- j is above i (going up on the row values)
 * (-1) - j is below i
 * 2 - j is to the right of i
 * (-2) j is to the left of i
 * 3 - j is upper right to i
 * (-3) j is down left to i
 * 4 - j is upper left to i
 * (-4) - j is down right to i
 */
int Relation(int i,int j);
/**
 * returns the position value of row,col. the value ranges from 0 to CH_GAME_N_COLUMNS*CH_GAME_N_ROWS-1
 * @param
 * col - the columns value
 * row - the row value
 * @return
 * (-1) - either row or col are invalid values.
 * pos - otherwise, give the position for col and row.
 */
int chGamePos(int row,int col);
/**
 * returns the row value of a given position value
 * it is assumed pos is a valid value (there is no check)
 * @param
 * pos - the position value to be checked
 * @return
 * row - the row value of pos
 */
int chGameRow(int i);
/**
 * returns the column value of a given position value
 * it is assumed pos is a valid value (there is no check)
 * @param
 * pos - the position value to be checked
 * @return
 * row - the column value of pos
 */
int chGameCol(int i);

CHGame* chGameCreate(int historySize){
	 CHGame* game	=	( CHGame*) malloc(sizeof( CHGame));
		if((game	==	NULL)||(historySize	==	0)){ //historysize is never 0 on default value
			printf("%schGameCreate\n",ERR_MEM);
			return NULL;
		}
		int i,j;
		for(i	=	0; i	<	CH_GAME_N_COLUMNS; i++){  //Initialize gameboard array
			for(j	=	0; j	<	CH_GAME_N_ROWS; j++){
				game->gameBoard[j][i]	=	CH_GAME_EMPTY_ENTRY;
			}
		}

		//just initializing the fields. default starting values and gameboard are set in
		//chGameStarting (moved out of chGameCreate in order to use create game for loading)
		game->isCheck=false;
		game->p1_king=-1;
		game->p2_king=-1;
		game->currentPlayer	=	1;
		game->humancolor=1;
		game->humanplayer=1;
		game->diff=2;
		CHThreatMapCreate(game->threatMap);
		game->move_list	=	chArrayListCreate(historySize);
		if(game->move_list	==	NULL){//error msg was sent in chArrayListCreate
			chGameDestroy(game);
			return NULL;
		}

		return game;

}

int chGameStarting(CHGame* src){
	if(src == NULL){
		printf("%schGameStarting\n",ERR_NULL);
		return 0;
	}
	int i,j;
	for(i	=	0; i	<	CH_GAME_N_COLUMNS; i++){  //Initialize gameboard array
		for(j	=	0; j	<	CH_GAME_N_ROWS; j++){
			src->gameBoard[j][i]	=	CH_GAME_EMPTY_ENTRY;
			if(	j	== 1){ //line of white pawns
				src->gameBoard[j][i]	=	CH_PLAYER_1_PAWN;
			}
			if(	j	== 6){ //line of black pawns
				src->gameBoard[j][i]	=	CH_PLAYER_1_PAWN-'a'+'A';
			}
		}
	}
	//setting up the other pieces
	src->gameBoard[0][0]	=	CH_PLAYER_1_ROOK;
	src->gameBoard[0][7]	=	CH_PLAYER_1_ROOK;
	src->gameBoard[0][1]	=	CH_PLAYER_1_KNIGHT;
	src->gameBoard[0][6]	=	CH_PLAYER_1_KNIGHT;
	src->gameBoard[0][2]	=	CH_PLAYER_1_BISHOP;
	src->gameBoard[0][5]	=	CH_PLAYER_1_BISHOP;
	src->gameBoard[0][3]	=	CH_PLAYER_1_QUEEN;
	src->gameBoard[0][4]	=	CH_PLAYER_1_KING;

	src->gameBoard[7][0]	=	CH_PLAYER_1_ROOK-'a'+'A';
	src->gameBoard[7][7]	=	CH_PLAYER_1_ROOK-'a'+'A';
	src->gameBoard[7][1]	=	CH_PLAYER_1_KNIGHT-'a'+'A';
	src->gameBoard[7][6]	=	CH_PLAYER_1_KNIGHT-'a'+'A';
	src->gameBoard[7][2]	=	CH_PLAYER_1_BISHOP-'a'+'A';
	src->gameBoard[7][5]	=	CH_PLAYER_1_BISHOP-'a'+'A';
	src->gameBoard[7][3]	=	CH_PLAYER_1_QUEEN-'a'+'A';
	src->gameBoard[7][4]	=	CH_PLAYER_1_KING-'a'+'A';
	src->isCheck=false;
	src->p1_king=4;
	src->p2_king=60;
	src->currentPlayer	=	1;
	CHThreatMapStarting(src->threatMap);

	//setting a clean move history
	if(chArrayListClear(src->move_list) != CH_ARRAY_LIST_SUCCESS){
		return 0;
	}
	return 1;

}

CHGame* chGameCopy(CHGame* src){
	if(src == NULL){
		printf("%schGameCopy\n",ERR_NULL);
		return NULL;
	}
 CHGame* game	=	( CHGame*) malloc(sizeof( CHGame));
	if((game	==	NULL)){
		printf("%schGameCopy\n",ERR_MEM);
		return NULL;
	}
	int i,j;
	for(i	=	0; i	<	CH_GAME_N_COLUMNS; i++){ //copy gameboard
		for(j	=	0; j	<	CH_GAME_N_ROWS; j++){
			game->gameBoard[j][i]	=	src->gameBoard[j][i];
		}
	}

	game->currentPlayer	=	src->currentPlayer;
	game->move_list	=	chArrayListCopy(src->move_list);
	CHThreatMapCopy(src->threatMap,game->threatMap);
	game->humanplayer = src->humanplayer;
	game->isCheck = src->isCheck;
	game->p1_king = src->p1_king;
	game->p2_king = src->p2_king;
	game->humancolor = src->humancolor;
	if(game->move_list	==	NULL){ //memory allocation error
		chGameDestroy(game);
		printf("%schGameCopy\n",ERR_MEM);
		return NULL;
	}
	return game;

}

void chGameDestroy(CHGame* src){
	if((src == NULL)){
		printf("%schGameDestroy\n",ERR_NULL);
		return;
	}
	chArrayListDestroy(src->move_list);
	free(src);
}




int chGameUpdateThreat(CHGame* src, int target){
	if((src == NULL)){
		printf("%schGameUpdateThreat\n",ERR_NULL);
		return 0;
	}
	if((chGameIsValidPos(target) == false)){
		return 2;
	}
	int i_tar	=	chGameRow(target);
	int j_tar	=	chGameCol(target);
	char piece	= src->gameBoard[i_tar][j_tar];

//updating threats blocked by the target position
	int i,j;
	for(i=0; i<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
		for(j=0;j<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;j++){
			if((Relation(i,target) != 0)&&(target != j)&&(Relation(i,target) == Relation(i,j))){ //i,j and target are different and on the same line. either: i-target-j or i-j-target
				if(Relation(i,target) == Relation(target,j)){ // i-target-j
					CHThreatMapUpdate(src->threatMap,j,i,false);
				}
			}
		}
	}

//update the map with the position threatened by the piece after it moved

	int mod=0;
	if(chGameGetColor(src,target) == 0){
	 mod='a'-'A';
	}
	switch(piece+mod){
		case CH_PLAYER_1_PAWN:
			UpdateThreatPawn(src,target,mod);
			break;
		case CH_PLAYER_1_BISHOP:
			UpdateThreatLine(src,target,min(i_tar,j_tar),-1,-1);  //down left
			UpdateThreatLine(src,target,min(i_tar,CH_GAME_N_COLUMNS-j_tar-1),-1,1);  //down right
			UpdateThreatLine(src,target,min(CH_GAME_N_ROWS-i_tar-1,j_tar),1,-1); //up left
			UpdateThreatLine(src,target,min(CH_GAME_N_ROWS-i_tar-1,CH_GAME_N_COLUMNS-j_tar-1),1,1);  //up right
			break;
		case CH_PLAYER_1_ROOK:
			UpdateThreatLine(src,target,i_tar,-1,0);  //down
			UpdateThreatLine(src,target,CH_GAME_N_COLUMNS-j_tar-1,0,1);  //right
			UpdateThreatLine(src,target,j_tar,0,-1); //left
			UpdateThreatLine(src,target,CH_GAME_N_ROWS-i_tar,1,0);  //up
			break;
		case CH_PLAYER_1_KNIGHT:
			UpdateThreatKnight(src, target);
			break;
		case CH_PLAYER_1_QUEEN:
			UpdateThreatLine(src,target,i_tar,-1,0);  //down
			UpdateThreatLine(src,target,CH_GAME_N_COLUMNS-j_tar-1,0,1);  //right
			UpdateThreatLine(src,target,j_tar,0,-1); //left
			UpdateThreatLine(src,target,CH_GAME_N_ROWS-i_tar,1,0);  //up
			UpdateThreatLine(src,target,min(i_tar,j_tar),-1,-1);  //down left
			UpdateThreatLine(src,target,min(i_tar,CH_GAME_N_COLUMNS-j_tar-1),-1,1);  //down right
			UpdateThreatLine(src,target,min(CH_GAME_N_ROWS-i_tar-1,j_tar),1,-1); //up left
			UpdateThreatLine(src,target,min(CH_GAME_N_ROWS-i_tar-1,CH_GAME_N_COLUMNS-j_tar-1),1,1);  //up right
			break;
		case CH_PLAYER_1_KING:
			UpdateThreatKing(src,target);
			break;
	}

	if(piece	==	CH_PLAYER_1_KING){
		src->p1_king	=	target;
	}
	if(piece	==	CH_PLAYER_1_KING-'a'+'A'){
			src->p2_king	=	target;
	}
	if(mod==0){ //the moved piece was white
		if(CHThreatMapCheck(src->threatMap,src->p2_king,target)==true){ //the moved piece is threatening the black king
			src->isCheck = true;
		}
	}
	else{
		if(CHThreatMapCheck(src->threatMap,src->p1_king,target)==true){ //the moved piece is threatening the white king
			src->isCheck = true;
		}
	}
	return 1;
}

CH_GAME_MESSAGE chGameSetMove(CHGame* src, int origin, int target){
    //setMove isn't checking if the move is valid in order to implement undo
	if((src == NULL)){
		printf("%schGameSetMove\n",ERR_NULL);
		return CH_GAME_INVALID_ARGUMENT;
	}
	if((chGameIsValidPos(origin) == false)||(chGameIsValidPos(target) == false)){
		return CH_GAME_INVALID_MOVE;
	}

	int i_tar	=	chGameRow(target);
	int j_tar	=	chGameCol(target);
	char piece	= src->gameBoard[chGameRow(origin)][chGameCol(origin)];
	char piece_taken	=	src->gameBoard[i_tar][j_tar];
	src->gameBoard[chGameRow(origin)][chGameCol(origin)]=CH_GAME_EMPTY_ENTRY;
	src->gameBoard[i_tar][j_tar]=piece;

	if(src->isCheck == true){ //the move is valid so it is not possible to stay in check
		src->isCheck = false;
	}

	///update threat map here
	//removing the threats by the origin position
	int i,j;
	for(i=0;i<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
		CHThreatMapUpdate(src->threatMap,i,origin,false);
	}

	//removing the threats by the target position (in case the move is an attack and not a regular move)
	for(i=0;i<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
		CHThreatMapUpdate(src->threatMap,i,target,false);
	}

	//updating threats "unblocked" by the current move
	int temp_rel,dir_col=0,dir_row=0;
	for (i=0; i<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
		if(CHThreatMapCheck(src->threatMap,origin,i) == true){
			if((IsPawn(src,i) == 2)&&(i != src->p1_king)&&((i != src->p2_king))){ //makes sure the threatening piece isn't a pawn or a king (which both have a range of 1.
				dir_col=0;
				dir_row=0;
				temp_rel= Relation(origin,i);
				switch(temp_rel){
					case 1: //above
							dir_row=-1;
						break;
					case -1:
						dir_row=1;
						break;
					case 2:
						dir_col=-1;
						break;
					case -2:
						dir_col=1;
						break;
					case 3:
						dir_col=-1;
						dir_row=-1;
						break;
					case -3:
						dir_col=1;
						dir_row=1;
						break;
					case 4:
						dir_col=1;
						dir_row=-1;
						break;
					case -4:
						dir_col=-1;
						dir_row=1;
						break;
				}
				if((dir_col != 0)||(dir_row != 0 )){
					for(j= origin; (j>=0)&&(j<=CH_GAME_N_COLUMNS*CH_GAME_N_ROWS -1);j+=dir_col+dir_row*CH_GAME_N_COLUMNS){

						if(src->gameBoard[chGameRow(j)][chGameCol(j)] == CH_GAME_EMPTY_ENTRY){
							CHThreatMapUpdate(src->threatMap,j,i,true);  //empty position threatened by i after origin moved
						}
						else{
							CHThreatMapUpdate(src->threatMap,j,i,true); //non empty position threathend by i. the rest is blocked by this postion.
							break;
						}

						//stops when reaching the edge of the board
						if((dir_col == 1)&&(chGameCol(j) == CH_GAME_N_COLUMNS-1)){
							break;
						}
						if((dir_col == -1)&&(chGameCol(j) == 0)){
							break;
						}
						if((dir_row == 1)&&(chGameRow(j) == CH_GAME_N_ROWS-1)){
							break;
						}
						if((dir_row == -1)&&(chGameRow(j) == 0)){
							break;
						}
					}
				}
			}
		}
	}

	chGameUpdateThreat(src,target);
	if(chArrayListIsFull(src->move_list)	==	true){ //move list is full, deleting the first move in the list before adding the new move.
		chArrayListRemoveFirst(src->move_list);
	}
	chArrayListAddLast(src->move_list,origin,target,(int)(piece_taken));
	if(src->currentPlayer	==	1){
		src->currentPlayer	=	0;
	}
	else{
		src->currentPlayer	=	1;
	}

	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE chGameIsValidMove(CHGame* src, int origin,int target){
	if((src == NULL)){
		printf("%schGameIsValidMove\n",ERR_NULL);
		return CH_GAME_INVALID_ARGUMENT;
	}
	if((chGameIsValidPos(origin) == false)||(chGameIsValidPos(target) == false)){
		return CH_GAME_INVALID_MOVE;
	}

	if(origin == target){ //"moving" to the same spot isn't a valid move (you can't skip turns in chess).
		return CH_GAME_INVALID_MOVE;
	}

	//coordinates of the origin position:
	int i_org=(int)(origin/CH_GAME_N_COLUMNS);
	int j_org=(int)(origin % CH_GAME_N_COLUMNS);
	//coordinates of the target position:
	int i_tar=(int)(target/CH_GAME_N_COLUMNS);
	int j_tar=(int)(target % CH_GAME_N_COLUMNS);
	char piece1=src->gameBoard[i_org][j_org];
	char piece2=src->gameBoard[i_tar][j_tar];
	if(src->currentPlayer==1){
		if((piece1<'a')||(piece1>'z')){ //check if the player is moving his own piece. return false if not.
			//even if the piece symbols changed(in the "defines"), we assume that player1 uses "small" letters as symbols.

			return CH_GAME_INVALID_PIECE;
		}
		if((piece2>='a')&&(piece2<='z')){ //check if the player is trying to eat his own piece

			return CH_GAME_INVALID_MOVE;
		}
	}
	if(src->currentPlayer==0){
		if((piece1<'A')||(piece1>'Z')){ //check if the player is moving his own piece
			//even if the piece values change, we assume that player2 use capital letters as symbols.
			return CH_GAME_INVALID_PIECE;
		}
		if((piece2>='A')&&(piece2<='Z')){ //check if the player is trying to eat his own piece

			return CH_GAME_INVALID_MOVE;
		}
	}

	if((IsPawn(src, origin) == 1)&&(piece2 == CH_GAME_EMPTY_ENTRY)){ //piece is white pawn and is moving (not attacking)

		if((j_org==j_tar)&&(i_org+1	== i_tar)){ //one move forward
			if((src->isCheck == true)){
				if((IsDefendCheck(src,origin,target) == false)){ //the player is in check and the move isn't defending the king

					return CH_GAME_STILL_CHECK;
				}
				else{
					return CH_GAME_SUCCESS;
				}
			}
			if(IsSelfCheck(src,origin,target) == false){ //the move doesn't cause a check to your own king
				return CH_GAME_SUCCESS;
			}

			return CH_GAME_WILL_CHECK;
		}

		if((i_org==1)&&(j_org==j_tar)&&(i_tar==3)&&(src->gameBoard[2][j_org] ==  CH_GAME_EMPTY_ENTRY)){
			//pawn is moving twice from starting position. path isn't blocked
			if((src->isCheck == true)){
				if((IsDefendCheck(src,origin,target) == false)){ //the player is in check and the move isn't defending the king
					return CH_GAME_STILL_CHECK;
				}
				else{
					return CH_GAME_SUCCESS;
				}
			}
			if(IsSelfCheck(src,origin,target) == false){
				return CH_GAME_SUCCESS;
			}

			return CH_GAME_WILL_CHECK;
		}

	return CH_GAME_INVALID_MOVE;
	}
	if((IsPawn(src, origin) == 0)&&(piece2 == CH_GAME_EMPTY_ENTRY)){ //piece is black pawn
		if((j_org==j_tar)&&(i_org-1	== i_tar)){ //one move forward
			if((src->isCheck == true)){
				if((IsDefendCheck(src,origin,target) == false)){ //the player is in check and the move isn't defending the king

					return CH_GAME_STILL_CHECK;
				}
				else{
					return CH_GAME_SUCCESS;
				}
			}
			if(IsSelfCheck(src,origin,target) == false){ //the move doesn't cause a check to your own king
				return CH_GAME_SUCCESS;
			}

			return CH_GAME_WILL_CHECK;
		}

		if((i_org==6)&&(j_org==j_tar)&&(i_tar==4)&&(src->gameBoard[5][j_org] ==  CH_GAME_EMPTY_ENTRY)){
			//pawn is moving twice from starting position. path isn't blocked
			if((src->isCheck == true)){
				if((IsDefendCheck(src,origin,target) == false)){ //the player is in check and the move isn't defending the king

					return CH_GAME_STILL_CHECK;
				}
				else{
					return CH_GAME_SUCCESS;
				}
			}
			if(IsSelfCheck(src,origin,target) == false){
				return CH_GAME_SUCCESS;
			}

			return CH_GAME_WILL_CHECK;
		}

		return CH_GAME_INVALID_MOVE;
	}
	if(CHThreatMapCheck(src->threatMap,target,origin) == true){
		if((src->isCheck == true)){
			if((IsDefendCheck(src,origin,target) == false)){ //the player is in check and the move isn't defending the king

				return CH_GAME_STILL_CHECK;
			}
			else{
				if(IsPawn(src,origin) != 2){ //piece is a pawn
					if(chGameGetColor(src,target) != 2){ //pawn can only move to the threathend position as an attack (not as a reguler move)
						return CH_GAME_SUCCESS;
					}
					else{


						return CH_GAME_INVALID_MOVE;
					}
				}
				return CH_GAME_SUCCESS;
			}
		}
		if(IsSelfCheck(src,origin,target) == false){
			if(IsPawn(src,origin) != 2){ //piece is a pawn
				if(chGameGetColor(src,target) != 2){ //pawn can only move to the threathend position as an attack (not as a reguler move)
					return CH_GAME_SUCCESS;
				}
				else{


					return CH_GAME_INVALID_MOVE;
				}
			}
			return CH_GAME_SUCCESS;
		}
	}  //end of: "if(CHThreatMapCheck(src->threatMap,target,origin) == true)"


	return CH_GAME_INVALID_MOVE;
}



int chGameGetColor(CHGame* src,int origin){ //1- white 0- black 2-empty  -1 illegal input
	if((src == NULL)){
		printf("%schGameGetColor",ERR_NULL);
		return -1;
	}
	if((chGameIsValidPos(origin) == false)){
		return -1;
	}
	int i,j;
	i= (int)(origin /CH_GAME_N_COLUMNS);
	j= (int)(origin %CH_GAME_N_COLUMNS);
	if(src->gameBoard[i][j] == CH_GAME_EMPTY_ENTRY){
		return 2;
	}
	if((src->gameBoard[i][j]>='a')&&(src->gameBoard[i][j]<='z')){
		return 1;
	}
	return 0;
}
CHMoveList chGameGetMoves(CHGame* src,int origin){
	CHMoveList mov;	//( CHMoveList*) malloc(sizeof( CHMoveList));
	if((src == NULL)){
		printf("%schGameGetMoves",ERR_NULL);
		mov.listSize = -1;
		return mov;
	}
	if((chGameIsValidPos(origin) == false)){
		mov.listSize = -2;
		return mov;
	}
	CHGame* cpy = NULL;
	if(chGameGetColor(src,origin) != src->currentPlayer){ //checking moves for the enemy player.
		cpy = chGameCopy(src);
		cpy->currentPlayer = chGameGetColor(src,origin);
		src = cpy;
	}

	mov.listSize=0;
	int i,j;
	int col,row;
	for(i=0; i <CH_GAME_N_ROWS*CH_GAME_N_COLUMNS;i++ ){
		mov.moveList[i] = 0;
	}
	for(i=0;i<CH_GAME_N_ROWS*CH_GAME_N_COLUMNS;i++){
		if(chGameIsValidMove(src,origin,i) == CH_GAME_SUCCESS){
			mov.moveList[mov.listSize]=i;
			mov.listSize++;
			col = chGameCol(i);
			row = chGameRow(i);
			for(j = 0; j< mov.listSize-1;j++ ){ //setting up so the mov with the lowest column and then the lowest row comes first
				if(col < chGameCol(mov.moveList[j])){
					mov.moveList[mov.listSize-1] = mov.moveList[j];
					mov.moveList[j] = i;
					break;
				}
				else{
					if(row < chGameRow(mov.moveList[j])){
						mov.moveList[mov.listSize-1] = mov.moveList[j];
						mov.moveList[j] = i;
						break;
					}
				}
			}
		}
	}
	if(cpy != NULL){
		chGameDestroy(cpy);
	}
	return mov;
}







CH_GAME_MESSAGE chGameUndoPrevMove(CHGame* src){
	if(src	==	NULL){
		printf("%schGameUndoPrevMove\n",ERR_NULL);
		return CH_GAME_INVALID_ARGUMENT;
	}

	if(chArrayListIsEmpty(src->move_list)){
		return CH_GAME_NO_HISTORY;
	}

	int* mov;
	int i_tar,j_tar;
	char piece;
	mov= chArrayListGetLast(src->move_list);
	if(mov != NULL){
		piece = (*(mov+2));
	}
	i_tar	=	(int)((*(mov+1))/CH_GAME_N_COLUMNS);
	j_tar	=	(int)((*(mov+1)) % CH_GAME_N_COLUMNS);

	chArrayListRemoveLast(src->move_list);
	chGameSetMove(src,*(mov+1),*mov);
	chArrayListRemoveLast(src->move_list);

	src->gameBoard[i_tar][j_tar]	= piece;
	chGameUpdateThreat(src,*mov+1);

	return CH_GAME_SUCCESS;
}


CH_GAME_MESSAGE chGamePrintBoard(CHGame* src, FILE* fout){
	if(src	==	NULL){
		printf("%schGamePrintBoard\n",ERR_NULL);
		return CH_GAME_INVALID_ARGUMENT;
	}
	if(fout == NULL){
		fout=stdout;
	}
	int i,j;
	for(i	=	CH_GAME_N_ROWS-1; i	>=	0; i--){
		fprintf(fout,"%d| ",i+1);
		for(j	=	0; j	<	CH_GAME_N_COLUMNS; j++){
			fprintf(fout,"%c ",src->gameBoard[i][j]);
		}
		fprintf(fout,"|\n");
	}
	printf("  ");
	for(j	=	0; j	<	2*CH_GAME_N_COLUMNS+1; j++){
		fprintf(fout,"-");
	}

	fprintf(fout,"\n  ");

	for(j	=	0; j	<	CH_GAME_N_COLUMNS; j++){
			fprintf(fout," %c",'A'+j);
		}

	fprintf(fout,"\n");
	return CH_GAME_SUCCESS;
}


char chGameCheckWinner(CHGame* src){
	if(src == NULL){
		printf("%schGameCheckWinner\n",ERR_NULL);
		return 0;
	}
	CHMoveList mov;
	int noMoves = 1; //indicator that no legal moves were found
	int i;
	for(i=0; i < CH_GAME_N_COLUMNS*CH_GAME_N_ROWS; i++ ){
		if(chGameGetColor(src,i) == src->currentPlayer){
			mov = chGameGetMoves(src,i);
			if(mov.listSize > 0){ // a legal move was found
				noMoves = 0;
				break;
			}
		}
	}
	if(noMoves == 0){
		return 'N'; // N for No winner and no Draw
	}
	if(src->isCheck == true){
		if(src->currentPlayer == 1){
			return CH_PLAYER_1_KING+'A'-'a';  // K for black player's victory
		}
		else{
			return CH_PLAYER_1_KING;   // k for white player's victory
		}
	}
	return CH_GAME_EMPTY_ENTRY;  // _ for Draw
}

int chGameSettingsPrint(CHGame* src, FILE* fout){
	if(src == NULL){
		printf("%schGameSettingsPrint",ERR_NULL);
		return 0;
	}
	if(fout == NULL){
		fout = stdout;
	}
	char diff[5][10]={{"amateur\n"},{"easy\n"},{"moderate\n"},{"hard\n"},{"expert\n"}};

	fprintf(fout,"GAME_MODE: %d-player\n",src->humanplayer);
	if(src->humanplayer==1){
		fprintf(fout,"DIFFICULTY: %s",diff[src->diff-1]);
		fprintf(fout,"USER_COLOR: ");
		if(src->humancolor == 1){
			fprintf(fout,"white\n");
		}
		else{
			fprintf(fout,"black\n");
		}
	}
	return 1;
}

bool chGameIsHumanTurn(CHGame* src){
	if(src == NULL){
		return false;
	}
	if(src->humanplayer == 2){
		return true;
	}
	if(src->currentPlayer == src->humancolor){
		return true;
	}
	return false;
}

bool chGameIsValidPos(int pos){
	if((pos < 0)||(pos >= CH_GAME_N_COLUMNS*CH_GAME_N_ROWS)){
		return false;
	}
	return true;
}


//various position relation functions.
int chGameCol(int i){ //gets a position (1 to 64) and returns the postion's column.no check for valid input is made.
	return (int)(i%CH_GAME_N_COLUMNS);
}
int chGameRow(int i){ //gets a position (1 to 64 for default values) and returns the postion's Row.no check for valid input is made.
	return (int)(i/CH_GAME_N_COLUMNS);
}
int chGamePos(int row,int col){  //gets a column and row and return the position (a value from 1 to 64) for a valid column and row. return -1 otherwise (invalid values are NOT errors)
	if((col >= 0)&&(col < CH_GAME_N_COLUMNS)&&(row >= 0) &&(row < CH_GAME_N_ROWS)){
		return row*CH_GAME_N_COLUMNS+col;
	}
	return -1;
}

bool IsDefendCheck(CHGame* src, int origin,int target){ //when the king is checked. checking if the move successfully defends the king
	if((src == NULL)||(chGameIsValidPos(origin) == false)||(chGameIsValidPos(target) == false)){
			return false;
	}
	int king_pos;
	int threat_pos=-1;
	if(src->currentPlayer == 1){
		king_pos=src->p1_king;
	}
	else{
		king_pos=src->p2_king;
	}
	int i;
	if(origin == king_pos){ //the king is moving
		for(i=0;i<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
			if(chGameGetColor(src,origin) != chGameGetColor(src,i)){ //an enemy piece is in position i
				if((CHThreatMapCheck(src->threatMap,target, i) == true)){ //the target position is threatened by position i
					return false;
				}
				else{
					if((CHThreatMapCheck(src->threatMap,origin, i) == true)&&(Relation(target,origin) == Relation(target, i))){
						//i is threatening the king and origin,target aned i are on the same line
						if((i != src->p1_king)&&(i != src->p2_king)&&(IsPawn(src,i) == 2)){
							//the piece in i isn't a king or a pawn (the only pieces that threaten in direct line with limited range)
							return false;
						}
					}
				}
			}
		}
		return true;
	}
	for(i=0;i<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
		if((CHThreatMapCheck(src->threatMap,king_pos, i) == true)&&(chGameGetColor(src,king_pos) != chGameGetColor(src,i) )){
			if(threat_pos == -1){
				threat_pos=i;
				break;
			}
			else{
				return false; //the king is checked by 2 pieces. can only be resolved with moving the king.
			}
		}
	}
	if(target == threat_pos){ //the piece is eating the threat. we assume a valid move check was made before.
		return true;
	}
	if((Relation(target,threat_pos) != 0)&&(Relation(target,threat_pos) == Relation(king_pos,threat_pos))){ //the target,king and threat are on the same line
		if(((target>king_pos)&&(target<threat_pos))||((target<king_pos)&&(target>threat_pos))){ // the target is between the king and threat
			return true;
		}
	}
	return false;
}
bool IsSelfCheck(CHGame* src, int origin,int target){  //check if the moves open the player's king to a check
	if((src == NULL)||(chGameIsValidPos(origin) == false)||(chGameIsValidPos(target) == false)){
			return true; //we want to turn down flawed input.
	}
	int king_pos,k,i,j;
	if(src->p1_king == origin){ //white king is moving
		for(k=0; k<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;k++){
			if((CHThreatMapCheck(src->threatMap,target, k) == true)&&(chGameGetColor(src,k) == 0)){
				return true;
			}
		}
		return false;
	}
	if(src->p2_king == origin){ //black king is moving
		for(k=0; k<CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;k++){
			if((CHThreatMapCheck(src->threatMap,target, k) == true)&&(chGameGetColor(src,k) == 1)){
				return true;
			}
		}
		return false;
	}

	if(src->currentPlayer == 1){
		king_pos=src->p1_king;
	}
	else{
		king_pos=src->p2_king;
	}
	if(Relation(origin,king_pos) == 0){ //the origin and king position aren't the same line. the move can't expose the king.
		return false;
	}
	for(i=0; i < CH_GAME_N_COLUMNS*CH_GAME_N_ROWS;i++){
		if((CHThreatMapCheck(src->threatMap,origin, i) == true)&&(target != i)
				&&(chGameGetColor(src,i) != chGameGetColor(src,king_pos))){ //the position ,i, can't threaten the king if it is eaten by the piece
			if(IsKnight(src,i) == 2){
				if(Relation(i,origin) == Relation(origin,king_pos)){  //pieces positioned as: i -- origin - king
					if(( Relation(i,target) != Relation(target,king_pos))
							||((Relation(i,target) == 0)&&Relation(target,king_pos) == 0)){ // NOT: i--target--king

						for(j=min(i,king_pos)+1;j < max(i,king_pos);j++){ //making sure there isn't another piece blocking the path to the king
							if(chGameGetColor(src,j) != 2){
								if((Relation(i,j) == Relation(j,king_pos))&&(Relation(i,j) != 0)&&(j != origin)){ //i--j--king_pos and j is not origin.
									if((origin ==12)&&(i==20)){
										printf("fail");
									}
									return false; // there is another piece blocking the path.
								}
							}
						}
						return true;
					}
				}
			}
		}
	}
	return false;
}

void UpdateThreatPawn(CHGame* src, int target,int mod){
	if(src == NULL){
		return;
	}
	if(mod != 0){
		mod = -1;
	}
	else{
		mod = 1;
	}
	int j_tar = chGameCol(target);
	int i_tar = chGameRow(target);
		CHThreatMapUpdate(src->threatMap,chGamePos(i_tar+mod,j_tar+1),target,true);
		CHThreatMapUpdate(src->threatMap,chGamePos(i_tar+mod,j_tar-1),target,true);
}
void UpdateThreatLine(CHGame* src, int target, int maxvalue, int dir1,int dir2){
	if(src == NULL){
		return;
	}
	int k;
	int col,row;
	for(k=1;k<=maxvalue;k++){
		row = chGameRow(target)+k*dir1;
		col = chGameCol(target)+k*dir2;
		CHThreatMapUpdate(src->threatMap,chGamePos(row,col),target,true);
		if(src->gameBoard[row][col] != CH_GAME_EMPTY_ENTRY){ //reached a non empty postions that blocks the movement
			break;
		}
	}
}

void UpdateThreatKnight(CHGame* src, int target){
	if(src == NULL){
		return;
	}
	int j_tar = chGameCol(target);
	int i_tar = chGameRow(target);
	int i,j;
	for(i = -2; i <= 2; i++){
		if(i != 0){
			if((i == 2)||(i == -2)){
				j= 1;
			}
			else{
				j = 2;
			}
			CHThreatMapUpdate(src->threatMap,chGamePos(i_tar+i,j_tar+j),target,true);
			CHThreatMapUpdate(src->threatMap,chGamePos(i_tar+i,j_tar-j),target,true);
		}
	}
}

void UpdateThreatKing(CHGame* src, int target){
	if(src == NULL){
		return;
	}
	int j_tar = chGameCol(target);
	int i_tar = chGameRow(target);
	int i,j;
	for(i = -1; i <= 1; i++){
		for(j = -1; j<=1; j++){
			if((j != 0)||(i != 0)){ //the king isn't threatening his own position.
				CHThreatMapUpdate(src->threatMap,chGamePos(i_tar+i,j_tar+j),target,true);
			}
		}
	}
}

int IsKnight(CHGame* src, int origin){ //2- false  1- white knight 0-black knight
	if((src == NULL)||(chGameIsValidPos(origin) == false)){
			return -1;
		}
	if(src->gameBoard[chGameCol(origin)][chGameRow(origin)] == CH_PLAYER_1_KNIGHT){
		return 1;
	}
	if(src->gameBoard[chGameCol(origin)][chGameRow(origin)] == CH_PLAYER_1_KNIGHT-'a'+'A'){
		return 0;
	}
	return 2;
}

int IsPawn(CHGame* src, int origin){
	if((src == NULL)||(chGameIsValidPos(origin) == false)){
			return -1;
	}
	int i,j;
	i	=	(int)(origin / CH_GAME_N_COLUMNS );
	j	=	(int)(origin % CH_GAME_N_COLUMNS );
	if(src->gameBoard[i][j] == CH_PLAYER_1_PAWN){
		return 1;
	}
	if(src->gameBoard[i][j] == CH_PLAYER_1_PAWN+'A'-'a'){
		return 0;
	}
	return 2;
}

int Relation(int i,int j){ //check for the relation between position i and j.
// the output: 1- j is vertically above i (-1)- j is vertically below i 2- j is horizontal to i and to it's right. (-2)- j is horizontal to i and to it's left
// 3- j is diagonal to the upper right of i. (-3)- j is diagonal to the down left of i  4- j is diagonal to the upper left of i (-4)- j is diagonal to the down left of i
// returns 0 otherwise ** i != j is assumed and isn't checked
	int i_col=chGameCol(i);
	int i_row=chGameRow(i);
	int j_col=chGameCol(j);
	int j_row=chGameRow(j);
	if(i_col == j_col){ // vertical relation
		if(i_row > j_row){
			return -1;
		}
		else{
			return 1;
		}
	}
	if(i_row == j_row){
		if(i_col > j_col){
			return -2;
		}
		else{
			return 2;
		}
	}
	if(((i_col-j_col)*(i_col-j_col)) == ((i_row-j_row)*(i_row-j_row))){ //diagonal relation
		if(i_col > j_col){ //j is to the left of i
			if(i_row > j_row){ //down left
				return -3;
			}
			else{  //upper left
				return 4;
			}
		}
		else{ //j is to the right of i
			if(i_row > j_row){ //down right
				return -4;
			}
			else{  //upper right
				return 3;
			}
		}
	}
	return 0;
}
