#include "CHMainAux.h"
#include <stdio.h>
#include "CHParser.h"
#include <stdlib.h>
#include <stdbool.h>
#include "CHGame.h"
#include "CHMinimax.h"
#include "CHThreatMap.h"
#include <string.h>


// private functions declarations ///
/**
 * gets the player's command.
 * the function doesn't quit until a valid command is recieved.
 * in case of an invalid command the function print an error massage and asks again for command.
 * @return
 * a valid SPCommand variable
 */
CHCommand ConsoleGetCommand(void);

/**
 * does the chess piece movements for console mode. including checking the move is legal.
 * @params
 * src - the game structure
 * cmd - the move command
 * isWon - pointer to be update if the game is over
 * @return
 * 0 - on error
 * 1 - otherwise (wheter the move was legal or not)
 */
int ConsoleMove(CHGame* src,CHCommand cmd,char* isWon);
/**
 * does the undo command for console mode.
 * @params
 * src - the game structure
 * @return
 * 0 - on error
 * 1 - otherwise
 */
int ConsoleUndo(CHGame* src);
/**
 * does the get_moves command for console mode.
 * @params
 * src - the game structure
 * origin - the pieces we are getting the legal moves for.
 * @return
 * 0 - on error
 * 1 - otherwise
 */
int ConsolePrintMoves(CHGame* src, int origin);

/// public functions implementations ///

CHGame* CHMainAuxInitialize(void){
	char* str	=	( char*) malloc(sizeof(char)*CH_MAX_LINE_LENGTH);
	char diff[5][10]={{"amateur\n"},{"easy\n"},{"moderate\n"},{"hard\n"},{"expert\n"}};
	int error_choice;
	CHCommandSet cmd;

	while(str	==	NULL){  //memory allocation error occurred
		printf("%sCHMainAuxInitialize\n",ERR_MEM);
		printf(ERR_CHOICE_CRIT);
		if(chParserLineError() == 1){ //quit
			return NULL;
		}
		str	=	( char*) malloc(sizeof(char)*CH_MAX_LINE_LENGTH);
	}

	CHGame* game=chGameCreate(CH_HISTORY_SIZE*2);
	while(game == NULL){ //memory allocation error occurred or history size input is 0.
		printf(ERR_CHOICE_CRIT);
		if(chParserLineError() == 1){ //quit
			free(str);
			return NULL;
		}
		game=chGameCreate(CH_HISTORY_SIZE*2);
	}

	while(chGameStarting(game) == 0){  //setting up the game to the starting positions.
	 //an error occurred
		printf(ERR_CHOICE);
		error_choice = chParserLineError();
		if(error_choice == 1){ //quit
			free(str);
			return NULL;
		}
	}
	printf(" Chess\n-------\n");
	printf(INIT_MSG);

	while(1){  //infinite loop until getting a valid command or value
		fgets(str,CH_MAX_LINE_LENGTH,stdin);
		cmd=chParserPraseLineSetting(str);
		switch(cmd.cmd){

		case CH_QUIT1:
			free(str);
			printf(EXT);
			chGameDestroy(game);
			return NULL;

		case CH_INVALID_LINE1:
			printf(ERR_INVALID_CMD);
			break;

		case CH_GAME_MODE:
			if((cmd.arg!=1)&&(cmd.arg!=2)){
				printf(ERR_GAME_MOD);
			}
			else{
				game->humanplayer=cmd.arg;
				printf(GAME_MOD);
				printf("%d-player\n",cmd.arg);
			}
			break;

		case CH_DIFFICULTY:
			if((cmd.arg>=1)&&(cmd.arg<=5)){
				game->diff=cmd.arg;
				printf(DIFF_SET);
				printf("%s",diff[cmd.arg-1]);
			}
			else{
				printf(ERR_DIFF);
			}
			break;

		case CH_USER_COLOR:
			if(game->humanplayer==2){
				printf(ERR_INVALID_CMD);
			}
			else{
				if((cmd.arg != 1)&&(cmd.arg != 2)){
					printf(ERR_COLOR);
				}
				else{
					game->humancolor	=	cmd.arg;
					printf(COLOR_SET);
					if(cmd.arg == 1){
						printf("white\n");
					}
					else{
						printf("black\n");
					}
				}
			}
			break;

		case CH_LOAD:
			if(CHMainAuxLoad(&game, cmd.path) == 0){
				printf(ERR_LOAD);
			}
			break;

		case CH_DEFAULT:
			game->diff=2;
			game->humanplayer=1;
			game->humancolor=1;
			break;

		case CH_PRINT_SETTING:
			printf("SETTINGS:\n");
			chGameSettingsPrint(game, NULL); //in case of an error we just
											//send a massage (from inside the function) and move on.
			break;

		case CH_START:
			free(str);
			printf(START);
			return game;
		}

	}
	return NULL; //line is never reached but compiler still gives a warning.
}



int CHMainAuxRunGameConsole(CHGame* src){
	if(src == NULL){
		printf("%sCHMainAuxRunGameConsole\n",ERR_NULL);
		return 0;
	}
	char isWon = 'N';
	CHCommand cmd;
	int done=0;

	chGamePrintBoard(src,NULL); //in case of an error we just send a massage (from inside the function) and move on.
	if((src->humanplayer == 2) || (src->currentPlayer == src->humancolor)){
		printf(NXT_MOV);
		if(src->currentPlayer == 1){
			printf("(white player):\n");
		}
		else{
			printf("(black player):\n");
		}
	}
	while(done == 0){
		cmd=ConsoleGetCommand(); //gets the player's command. in case of an error.
								//a massagge is sent from ConsoleGetCommand. the cmd is treated as an invalid command

		switch(cmd.cmd){
			case CH_QUIT:
				done=1;
				printf(EXT);
				break;
			case CH_RESTART:
				chGameDestroy(src);
				src=CHMainAuxInitialize();
				if(src == NULL){ //error are handled inside CHMainAuxInitialize. src=NULL can also be in case of a quit command.
					done =1;
				}
				else{
					chGamePrintBoard(src,NULL);
				}
				break;
			case CH_MOVE:
				done = ConsoleMove(src,cmd,&isWon); //in case of an error a massage is sent from ConsoleMove.
				break;	 	 	 	 	 	 	 	 	 		 	 	 	 //nothing else is done (like invalid commnad)
			case CH_GET_MOVE:
				if(cmd.validArg == true){
					if(chGameGetColor(src,cmd.arg1) == 2){ //the position is empty
						printf(ERR_EMPTY_POS);
					}
					else{
						ConsolePrintMoves(src,cmd.arg1);
					}
				}
				else{
					printf(ERR_INVALID_POS);
				}
				break;
			case CH_SAVE:
				if(CHMainAuxSave(src, cmd.path) == 0){
					printf(ERR_SAVE);
				}
				else{
					printf("Game saved to: %s\n", cmd.path);
				}
				break;
			case CH_UNDO_MOVE:
				ConsoleUndo(src);
				break;
			case CH_INVALID_LINE:
				printf(ERR_INVALID_CMD);
				break;
		}
	}
	switch(isWon){
	case 'k':
		printf(W_WON);
		break;
	case 'K':
		printf(B_WON);
		break;
	case '_':
		printf(TIE);
	}
	return 1;
}

int CHMainAuxRunGameGUI(void){
	CHGame* src = NULL;

	// initialize SDL2 for video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 0;
	}

	// create an SDL window
	SDL_Window* window = SDL_CreateWindow(
		"Main menu",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		600,
		600,
		SDL_WINDOW_SHOWN); //using SDL_WINDOW_SHOWN according to the forum

	// make sure window was created successfully
	if (window == NULL ) {
		printf("ERROR: unable to create window: %s\n", SDL_GetError());
		//a box massage and a choice will be given from chGUIStartWindow. i didn't want to make ErrorBox a public function
		//just for one check.
	}


	int check=1;

	int old_check=-1;
	do{
		switch(check){
		//note: all errors for the windows (besides the 1st SDL_CreateWindow) are handled from inside the window functions.
		case 1:
			check=chGUIStartWindow(&window);
			old_check=1;
			break;
		case 2:
			src=chGameCreate(CH_HISTORY_SIZE*2);
			if(src == NULL){
				//an error occured.

				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
			if(chGameStarting(src) == 0){  //setting up the game to the starting positions.
			 //an error occurred
				chGameDestroy(src);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
			check=chGUISettingWindow(window,&src);
			break;
		case 3:
			check=chGuiGameWindow(window,&src);
			old_check=3;
			break;
		case 4: //load (save is only called directly from the game window)
			check = chGuiSaveLoadWindow(1,&src);
			if( check == 1){  //load success
				check = 3;
			}
			else{
				check = old_check;
			}
			break;
		}
	}while(check != 0);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 1;

}

int CHMainAuxSave(CHGame* src, const char* fout){
	if(src == NULL){
		printf("%sCHMainAuxSave\n",ERR_NULL);
		return 0;
	}
	FILE* f= fopen(fout,"w+");
	if(f == NULL){
		return 0;
	}
	fprintf(f,"Current turn: ");
	if(src->currentPlayer == 1){
		fprintf(f,"white\n");
	}
	else{
		fprintf(f,"black\n");
	}
	if(chGameSettingsPrint(src,f) == 0){
		fclose(f);
		return 0;
	}
	if(chGamePrintBoard(src,f) == 0){
		fclose(f);
		return 0;
	}

	fclose(f);
	return 1;
}

int CHMainAuxLoad(CHGame** src, const char* fin){
	if(src == NULL){
		printf("%sCHMainAuxLoad\n",ERR_NULL);
	}
	CHGame* temp_game = chGameCreate(CH_HISTORY_SIZE*2);
	if(temp_game == NULL){ //game create error
		return 0;
	}

	FILE* f= fopen(fin,"r");
	if(f == NULL){ //couldn't open file
		chGameDestroy(temp_game);
		return 0;
	}
	char buff[255];
	fgets(buff,255,f); //current turn
	if(strcmp(buff,"Current turn: white\n") == 0){
		(temp_game)->currentPlayer=1;
	}
	else{
		(temp_game)->currentPlayer=0;
	}

	fgets(buff,255,f); //game mode
	if(strcmp(buff,"GAME_MODE: 1-player\n") == 0){
		(temp_game)->humanplayer = 1;

		fscanf(f,"%s",buff); //"DIFFICULTY:" *according the instructions we can simply assume correct file format.
		 // if i remember correctly there is no need to check if the file is correct or exists. **NEED TO RECHECK PROJECT REQUIRMENTS!!!**
		fgets(buff,255,f);
		switch(buff[0]){
		case 'a':
			(temp_game)->diff=1;
			break;
		case 'e':
			if(buff[1] == 's'){
				(temp_game)->diff=2;
			}
			else{
				(temp_game)->diff=5;
			}
			break;
		case 'm':
			(temp_game)->diff=3;
			break;
		case 'h':
			(temp_game)->diff=4;
			break;
		}

		fgets(buff,255,f); //user color
		if(strcmp(buff,"USER_COLOR: white\n") == 0){
			(temp_game)->humancolor = 1;
		}
		else{
			(temp_game)->humancolor = 0;
		}
	}

	if(strcmp(buff,"GAME_MODE: 2-player\n") == 0){
		(temp_game)->humanplayer = 2;
	}

	//get the positions on the board
	int i=0,j=0;
	for(i=0; i < CH_GAME_N_ROWS; i++){
		fgets(buff,255,f);
		for(j=0; j < CH_GAME_N_COLUMNS; j++ ){
			(temp_game)->gameBoard[CH_GAME_N_ROWS-i-1][j] = buff[3+2*j];
			//printf("%c",buff[3+2*j]);
			chGameUpdateThreat((temp_game),chGamePos(CH_GAME_N_ROWS-i-1,j));
		}
	}
	fgets(buff,255,f); // the "-------" line
	fgets(buff,255,f); // the "A B C ... " line

	fclose(f);
	CHGame* temp2;
	temp2 = *src;
	*src = temp_game;
	if(temp2 != NULL){
		chGameDestroy(temp2);
	}
	return 1;
}


/// private functions implementations ///

CHCommand ConsoleGetCommand(void){
	char* str	=	( char*) malloc(sizeof(char)*CH_MAX_LINE_LENGTH);
	CHCommand command;

	if(str	==	NULL){//memory allocation error occured
		printf("%sConsoleGetCommand\n",ERR_MEM);
		command.cmd=CH_INVALID_LINE;  //setting up a command the function will never normally  return in order to signal memory error to main.
		command.validArg=true;
		return command;
	}

	while(1){ //infinite loop until getting a valid command

		fgets(str,CH_MAX_LINE_LENGTH,stdin);
		command	=	chParserPraseLineGame(str);
		if(command.cmd	!=	CH_INVALID_LINE){
			free(str);
			return command;
		}
		printf(ERR_INVALID_CMD);
	}

	return command; //this line is never reached but complier gives a warning otherwise.
}


int ConsoleMove(CHGame* src,CHCommand cmd,char* isWon){
	if(src == NULL){
		printf("%sConsoleMove\n",ERR_NULL);
		return 0;
	}
	CH_GAME_MESSAGE result;
	int mov_org,mov_tar;
	int mod=0;
	int done=0;
	char Col[]={'A','B','C','D','E','F','G','H'};
	if((cmd.arg2 < 0)||(cmd.arg2 > CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)){ //position argument must be in the board range to be vaild
		cmd.validArg = false;
	}
	if(cmd.validArg == true){
		result = chGameIsValidMove(src,cmd.arg1,cmd.arg2);
		if( result == CH_GAME_SUCCESS){
			chGameSetMove(src,cmd.arg1,cmd.arg2);
			*isWon = chGameCheckWinner(src);
			if(*isWon == 'N'){ //game still not done
				if(src->isCheck == true){
					if(src->currentPlayer == 1){
						printf("Check: white king is threatened\n");
					}
					else{
						printf("Check: black king is threatened\n");
					}
				}
				if(chGameIsHumanTurn(src) == false){ // pc turn
					chMinimaxSuggestMove(src,(unsigned int) src->diff,&mov_org,&mov_tar);
					if(src->currentPlayer != 1){
						mod = 'a'-'A';
					}
					chGameSetMove(src, mov_org,mov_tar);
					printf("Computer: move ");
					switch(src->gameBoard[chGameRow(mov_tar)][chGameCol(mov_tar)]+mod){
					case CH_PLAYER_1_PAWN:
						printf("pawn ");
						break;
					case CH_PLAYER_1_BISHOP:
						printf("bishop ");
						break;
					case CH_PLAYER_1_ROOK:
						printf("rook ");
						break;
					case CH_PLAYER_1_KNIGHT:
						printf("knight ");
						break;
					case CH_PLAYER_1_QUEEN:
						printf("queen ");
						break;
					case CH_PLAYER_1_KING:
						printf("king ");
						break;
					}
					printf("at <%d,%c> to <%d,%c>\n", chGameRow(mov_org)+1,Col[chGameCol(mov_org)],chGameRow(mov_tar)+1,Col[chGameCol(mov_tar)]);
					*isWon = chGameCheckWinner(src);
					if(*isWon != 'N'){
						done=1;
					}
					else{
						if(src->isCheck == true){
							if(src->currentPlayer == 1){
								printf("Check: white king is threatened\n");
							}
							else{
								printf("Check: black king is threatened\n");
							}
						}
					}
				}

			}
			else{
				done=1;
			}
			if(done == 0){ //not done yet. print updated board and ask for next move.
				chGamePrintBoard(src,NULL);
				printf(NXT_MOV);
				if(src->currentPlayer == 1){
					printf("(white player):\n");
				}
				else{
					printf("(black player):\n");
				}
			}
		}
		else{ //the wanted move by the player is NOT a valid move
			switch(result){
			case CH_GAME_INVALID_MOVE:
				printf(ERR_MOV);
				break;
			case CH_GAME_INVALID_PIECE:
				printf(ERR_INVALID_PIECE);
				break;
			case CH_GAME_STILL_CHECK:
				printf(ERR_MOV_CHK_1);
				break;
			case CH_GAME_WILL_CHECK:
				printf(ERR_MOV_CHK_2);
				break;
			default:
				break;
			}
			printf(NXT_MOV);
			if(src->currentPlayer == 1){
				printf("(white player):\n");
			}
			else{
				printf("(black player):\n");
			}
		}
	}
	else{
		printf(ERR_INVALID_POS);
		printf(NXT_MOV);
		if(src->currentPlayer == 1){
			printf("(white player):\n");
		}
		else{
			printf("(black player):\n");
		}
	}
	return done;
}
int ConsoleUndo(CHGame* src){
	if(src == NULL){
		return 0;
	}
	char Col[]={'A','B','C','D','E','F','G','H'};
	int* undomove;
	if(src->move_list->actualSize > 0){
		undomove = chArrayListGetLast(src->move_list);
		if(src->currentPlayer	==	1){
			printf("Undo move for black player: <%d,%c> -> <%d,%c>\n",chGameRow(undomove[0])+1,Col[chGameCol(undomove[0])],chGameRow(undomove[1])+1,Col[chGameCol(undomove[1])] );
		}
		else{
			printf("Undo move for white player: <%d,%c> -> <%d,%c>\n",chGameRow(undomove[0])+1,Col[chGameCol(undomove[0])],chGameRow(undomove[1])+1,Col[chGameCol(undomove[1])] );
		}
		chGameUndoPrevMove(src);
	}
	if(src->move_list->actualSize > 0){
		undomove = chArrayListGetLast(src->move_list);
		if(src->currentPlayer	==	1){
			printf("Undo move for black player: <%d,%c> -> <%d,%c>\n",chGameRow(undomove[0])+1,Col[chGameCol(undomove[0])],chGameRow(undomove[1])+1,Col[chGameCol(undomove[1])] );
		}
		else{
			printf("Undo move for white player: <%d,%c> -> <%d,%c>\n",chGameRow(undomove[0])+1,Col[chGameCol(undomove[0])],chGameRow(undomove[1])+1,Col[chGameCol(undomove[1])] );
		}
		chGameUndoPrevMove(src);
	}
	chGamePrintBoard(src,NULL);
	return 1;
}

int ConsolePrintMoves(CHGame* src, int origin){
	if(src == NULL){
		printf("%sConsolePrintMoves",ERR_NULL);
		return 0;
	}
	int color= chGameGetColor(src,origin);

	CHMoveList mov_lst;
	int IsThreatened = 0;
	int temp;
	char Col[]={'A','B','C','D','E','F','G','H'};
	mov_lst =chGameGetMoves(src, origin);
	int i=0,j=0;
	for(i=0; i < mov_lst.listSize;i++){ //in case of an error with get moves listsize is -1 and the function won't enter the for loop.
		temp = mov_lst.moveList[i];
		for(j=0; j< CH_GAME_N_ROWS*CH_GAME_N_COLUMNS; j++){
			if(chGameGetColor(src,j) != color){
				if(CHThreatMapCheck(src->threatMap,temp, j) == true){
					IsThreatened = 1;
					break;
				}
			}
		}
		if(chGameGetColor(src,temp) == 2){ //not a capture
			if(IsThreatened == 1){
				printf("<%d,%c>*\n",chGameRow(temp)+1,Col[chGameCol(temp)]);
			}
			else{
				printf("<%d,%c>\n",chGameRow(temp)+1,Col[chGameCol(temp)]);
			}
		}
		else{ // a capture
			if(IsThreatened == 1){
				printf("<%d,%c>*^\n",chGameRow(temp)+1,Col[chGameCol(temp)]);
			}
			else{
				printf("<%d,%c>^\n",chGameRow(temp)+1,Col[chGameCol(temp)]);
			}
		}
	}
	return 1;
}
