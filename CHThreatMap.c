
#include "CHThreatMap.h"
#include "CHGame.h"
#include <stdlib.h>

int CHThreatMapCreate(int* map){ //initial create is empty in order to implement load
	if(map == NULL){
		printf("%sCHThreatMapCreate\n",ERR_NULL);
		return 0;
	}
	int max	=	(int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1;
	int i;
	for(i=0; i<max; i++){ //initialize the threat map
		map[i]	=	0;
	}
	return 1;

}

int CHThreatMapStarting(int* map){
//setup the initial threat map according to the starting positions on the board.
	if(map == NULL){
		printf("%sCHThreatMapStarting\n",ERR_NULL);
		return 0;
	}
	int max	=	(int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1;
	int i;
	for(i=0; i<max; i++){ //initialize the threat map
		map[i]=0;
	}
	//**couldn't come up with a method besides updating the map one by one.
	CHThreatMapUpdate(map,48,56,true); //black ROOK on 8A threatens 7A
	CHThreatMapUpdate(map,57,56,true); //black ROOK on 8A threatens 8B
	CHThreatMapUpdate(map,40,57,true); //black KNIGHT on 8B threatens 6A
	CHThreatMapUpdate(map,42,57,true); //black KNIGHT on 8B threatens 6C
	CHThreatMapUpdate(map,51,57,true); //black KNIGHT on 8B threatens 7D
	CHThreatMapUpdate(map,49,58,true); //black BISHOP on 8C threatens 7B
	CHThreatMapUpdate(map,51,58,true); //black BISHOP on 8C threatens 7D
	CHThreatMapUpdate(map,58,59,true); //black QUEEN on 8D threatens 8C
	CHThreatMapUpdate(map,50,59,true); //black QUEEN on 8D threatens 7C
	CHThreatMapUpdate(map,51,59,true); //black QUEEN on 8D threatens 7D
	CHThreatMapUpdate(map,52,59,true); //black QUEEN on 8D threatens 7E
	CHThreatMapUpdate(map,60,59,true); //black QUEEN on 8D threatens 8E
	CHThreatMapUpdate(map,59,60,true); //black KING on 8E threatens 8D
	CHThreatMapUpdate(map,51,60,true); //black KING on 8E threatens 7D
	CHThreatMapUpdate(map,52,60,true); //black KING on 8E threatens 7E
	CHThreatMapUpdate(map,53,60,true); //black KING on 8E threatens 7F
	CHThreatMapUpdate(map,61,60,true); //black KING on 8E threatens 8F
	CHThreatMapUpdate(map,52,61,true); //black BISHOP on 8F threatens 7E
	CHThreatMapUpdate(map,54,61,true); //black BISHOP on 8F threatens 7G
	CHThreatMapUpdate(map,45,62,true); //black KNIGHT on 8G threatens 6F
	CHThreatMapUpdate(map,47,62,true); //black KNIGHT on 8G threatens 6H
	CHThreatMapUpdate(map,52,62,true); //black KNIGHT on 8G threatens 7E
	CHThreatMapUpdate(map,55,63,true); //black ROOK on 8H threatens 7H
	CHThreatMapUpdate(map,62,63,true); //black ROOK on 8H threatens 8G
	CHThreatMapUpdate(map,41,48,true); //black PAWN on 7A threatens 6B
	CHThreatMapUpdate(map,40,49,true); //black PAWN on 7B threatens 6A
	CHThreatMapUpdate(map,42,49,true); ///black PAWN on 7B threatens 6C
	CHThreatMapUpdate(map,41,50,true); //black PAWN on 7C threatens 6B
	CHThreatMapUpdate(map,43,50,true); //black PAWN on 7C threatens 6D
	CHThreatMapUpdate(map,42,51,true); //black PAWN on 7D threatens 6C
	CHThreatMapUpdate(map,44,51,true); //black PAWN on 7D threatens 6E
	CHThreatMapUpdate(map,43,52,true); //black PAWN on 7E threatens 6D
	CHThreatMapUpdate(map,45,52,true); //black PAWN on 7E threatens 6F
	CHThreatMapUpdate(map,44,53,true); //black PAWN on 7F threatens 6E
	CHThreatMapUpdate(map,46,53,true); //black PAWN on 7F threatens 6G
	CHThreatMapUpdate(map,45,54,true); //black PAWN on 7G threatens 6F
	CHThreatMapUpdate(map,47,54,true); //black PAWN on 7G threatens 6H
	CHThreatMapUpdate(map,46,55,true); //black PAWN on 7H threatens 6G


	CHThreatMapUpdate(map,8,0,true); //white ROOK on 1A threatens 2A
	CHThreatMapUpdate(map,1,0,true); //white ROOK on 1A threatens 1B
	CHThreatMapUpdate(map,16,1,true); //white KNIGHT on 1B threatens 3A
	CHThreatMapUpdate(map,18,1,true); //white KNIGHT on 1B threatens 3C
	CHThreatMapUpdate(map,11,1,true); //white KNIGHT on 1B threatens 2D
	CHThreatMapUpdate(map,9,2,true); //white BISHOP on 1C threatens 2B
	CHThreatMapUpdate(map,11,2,true); //white BISHOP on 1C threatens 2D
	CHThreatMapUpdate(map,2,3,true); //white QUEEN on 1D threatens 1C
	CHThreatMapUpdate(map,10,3,true); //white QUEEN on 1D threatens 2C
	CHThreatMapUpdate(map,11,3,true); //white QUEEN on 1D threatens 2D
	CHThreatMapUpdate(map,12,3,true); //white QUEEN on 1D threatens 2E
	CHThreatMapUpdate(map,4,3,true); //white QUEEN on 1D threatens 1E
	CHThreatMapUpdate(map,3,4,true); //white KING on 1E threatens 1D
	CHThreatMapUpdate(map,11,4,true); //white KING on 1E threatens 2D
	CHThreatMapUpdate(map,12,4,true); //white KING on 1E threatens 2E
	CHThreatMapUpdate(map,13,4,true); //white KING on 1E threatens 2F
	CHThreatMapUpdate(map,5,4,true); //white KING on 1E threatens 1F
	CHThreatMapUpdate(map,12,5,true); //white BISHOP on 1F threatens 2E
	CHThreatMapUpdate(map,14,5,true); //white BISHOP on 1F threatens 2G
	CHThreatMapUpdate(map,21,6,true); //white KNIGHT on 1G threatens 3F
	CHThreatMapUpdate(map,23,6,true); //white KNIGHT on 1G threatens 3H
	CHThreatMapUpdate(map,12,6,true); //white KNIGHT on 1G threatens 2E
	CHThreatMapUpdate(map,15,7,true); //white ROOK on 1H threatens 2H
	CHThreatMapUpdate(map,6,7,true); //white ROOK on 1H threatens 1G
	CHThreatMapUpdate(map,17,8,true); //white PAWN on 2A threatens 3B
	CHThreatMapUpdate(map,16,9,true); //white PAWN on 2B threatens 3A
	CHThreatMapUpdate(map,18,9,true); ///white PAWN on 2B threatens 3C
	CHThreatMapUpdate(map,17,10,true); //white PAWN on 2C threatens 3B
	CHThreatMapUpdate(map,19,10,true); //white PAWN on 2C threatens 3D
	CHThreatMapUpdate(map,18,11,true); //white PAWN on 2D threatens 3C
	CHThreatMapUpdate(map,20,11,true); //white PAWN on 2D threatens 3E
	CHThreatMapUpdate(map,19,12,true); //white PAWN on 2E threatens 3D
	CHThreatMapUpdate(map,21,12,true); //white PAWN on 2E threatens 3F
	CHThreatMapUpdate(map,20,13,true); //white PAWN on 2F threatens 3E
	CHThreatMapUpdate(map,22,13,true); //white PAWN on 2F threatens 3G
	CHThreatMapUpdate(map,21,14,true); //white PAWN on 2G threatens 3F
	CHThreatMapUpdate(map,23,14,true); //white PAWN on 2G threatens 3H
	CHThreatMapUpdate(map,22,15,true); //white PAWN on 2H threatens 3G
	return 1;
}

int CHThreatMapCopy(int* src,int* cpy){
	if((src == NULL)||(cpy == NULL)){
		printf("%sCHThreatMapCopy\n",ERR_NULL);
		return 0;
	}
	int max	=	(int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1;
	int i;
	for(i=0;	i<max;	i++){
		cpy[i] = src[i];
	}
	return 1;
}



bool CHThreatMapCheck(int *src,int pos, int threat){
	if(src == NULL){ //null pointer error
		printf("%sCHThreatMapCheck\n",ERR_NULL);
		return false;
	}
	if((chGameIsValidPos(pos) == false)||(chGameIsValidPos(threat) == false)){ //input positions aren't valid(this isn't
																				//considered an error here but a feature)
		return false;
	}
	int chk	= (int)((pos*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)+threat)/32);
	threat	= threat % 32;
	int flag	=	1;
	flag = flag << (threat);
	if(src[chk] & flag){ //threat(the position) is threating pos.
		return true;
	}
	return false;
}

int CHThreatMapUpdate(int* src,int pos, int threat,bool value){
	if(src == NULL){
		printf("%sCHThreatMapUpdate\n", ERR_NULL);
		return 0;
	}
	if((chGameIsValidPos(pos) == true)&&(chGameIsValidPos(threat) == true)){ //check that pos and threat are valid gameboard positions
		int chk= (int)((pos*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)+threat)/32);
		threat= threat % 32;
		unsigned int flag	=	1;
		flag = flag << threat;
		if(value == true){
			src[chk] = src[chk] | flag;
		}
		else{
			flag = ~flag;
			src[chk] = src[chk] & flag;
		}
	}
	else{ //not valid positions
		if((pos == -1)||(threat == -1)){ //during a move update the chGamePos is entered as an argument for pos and threat.
			return 1;                   //it returns -1 for an invalid position so this is a check and not an error.
		}
		printf("%sCHThreatMapUpdate\n", ERR_POS_FUNC);

		return 0;
	}
	return 1;
}
