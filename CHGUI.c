#include "CHGUI.h"
#include <stdlib.h>
#include "CHThreatMap.h"
#include <string.h>
#include "CHMinimax.h"



//private function declarations//

/**
 * check if coordinates x,y are within rect
 *
 * @param
 * x,y - the coordinates to be checked
 * rect - the  SDL_Rect object to be checked
 * @return
 * 1 - point <x,y> is within rect
 * 0 - otherwise
 */
int inRect(int x, int y, SDL_Rect rect);
/**
 * a wrapper for SDL_DestroyTexture. the function checks tex != NULL before calling SDL_DestroyTexture.
 *
 * @param
 * tex - the texture to be destroyed
 */
void DestroyTextureWrap(SDL_Texture* tex);
/**
 * a wrapper for SDL_FreeSurface. the function make sure that *surf == NULL after the surface is freed.
 *
 * @param
 * surf - the texture to be destroyed
 */
void FreeSurfaceWrap(SDL_Surface** surf);
/**
 * a wrapper for SDL_CreateTextureFromSurface. after calling SDL_CreateTextureFromSurface, the functions check
 * if (tex == NULL) and print an error massage if true.
 *
 * @param
 * surf - the surface to be turned into texture
 * rend - the renderer for the window
 * @return
 * tex - the pointer to the texture that was created
 */
SDL_Texture* CreateTextureWrap(SDL_Renderer* rend, SDL_Surface* surf);
/**
 * a wrapper for SDL_LoadBMP. after calling SDL_LoadBMP, the functions check
 * if (tex == NULL) and print an error massage if true.
 *
 * @param
 * file - the file path of the bmp to be loaded
 * @return
 * surf - the pointer to the surface that was created
 */
SDL_Surface* LoadBMPWrap(const char* file);
/**
 * the function is called in case of an error. the function shows a massage box with some basic
 * information regarding the error (according to err_num) and give the player the options:
 * quit - exit the entire program
 * try again- try calling the function that caused to error again and check if it was successful
 * ignore - this options only available if can_ignore = 1. pressing this option will skip the function
 * and attempt to run the game anyway. (will mostly "work" for images that failed to load)
 *
 * @param
 * err_num - indicates the error massage to be put into the massage box
 * can_ignore - 0- ignore is not an option  1- ignore is an option.
 * @return
 * 1 - quit
 * 2 - try again
 * 3 - ignore
 */
int ErrorBox(int err_num, int can_ignore);
/**
 * the function is called in case the player wants to leave the game without saving.
 * the function shows a massage box that asks the player if it wants to save the game.
 * the options:
 * no - leave the game (whether through quit,main_menu, restart and etc)
 * yes - call the save window and then leave the game (after the window is done)
 * cancel - return to the game window
 *
 * @return
 * 3 - yes
 * 2 - no
 * 1 - cancel
 */
int SaveBox(void);
/**
 * the function is called in case the game is over.
 * the function shows a massage box that tells the player who won (white,black or tie)
 * the options:
 * ok - returns to the main menu
 *
 * @return
 * 0 - error occurred showing the massage box
 * 1 - otherwise
 */
int GameEndBox(int i);

//public functions implementations//

int chGUIStartWindow(SDL_Window** window){
	//check for correct input
	if(window == NULL){
		printf("%schGUIStartWindow",ERR_NULL);
		SDL_Quit();
		return 0;
	}
	int error_choice;
	while(*window == NULL){ //window didn't start correctly
		error_choice = ErrorBox(4,0);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			SDL_Quit();
			return 0;
		}
		*window = SDL_CreateWindow(
				"Main menu",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				600,
				600,
				SDL_WINDOW_SHOWN); //using SDL_WINDOW_SHOWN according to the forum

	}

	SDL_SetWindowTitle(*window,"Main menu");
	SDL_SetWindowSize(*window,600,600);

	// create a renderer for the window
		SDL_Renderer* rend = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
		while (rend == NULL) {
			printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
			error_choice = ErrorBox(1,0);
			if(error_choice == 1){
				SDL_DestroyWindow(*window);
				SDL_Quit();
				return 0;
			}
			rend = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
		}

	// loading images to surfaces
	SDL_Surface* surf_new =LoadBMPWrap("./images/Options/NewGame.bmp");
	SDL_Surface* surf_load =LoadBMPWrap("./images/Options/LoadGame.bmp");
	SDL_Surface* surf_quit =LoadBMPWrap("./images/Options/Quit.bmp");

	while ((surf_new == NULL)||(surf_load == NULL)||(surf_quit == NULL)) {
		//loading failed
		error_choice = ErrorBox(0,1);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			FreeSurfaceWrap(&surf_quit);
			FreeSurfaceWrap(&surf_load);
			FreeSurfaceWrap(&surf_new);
			SDL_DestroyRenderer(rend);
			SDL_DestroyWindow(*window);
			SDL_Quit();
			return 0;
		}
		if(error_choice == 3){ //ignore
			break;
		}
		if(surf_new == NULL){
			surf_new =LoadBMPWrap("./images/Options/NewGame.bmp");
		}
		if(surf_load == NULL){
			surf_load =LoadBMPWrap("./images/Options/LoadGame.bmp");
		}
		if(surf_quit == NULL){
			surf_quit =LoadBMPWrap("./images/Options/Quit.bmp");
		}
	}

	//creating textures from surfaces
	SDL_Texture* tex_new = CreateTextureWrap(rend,surf_new);
	SDL_Texture* tex_load = CreateTextureWrap(rend,surf_load);
	SDL_Texture* tex_quit = CreateTextureWrap(rend,surf_quit);



	while((tex_new == NULL)||(tex_load == NULL)||(tex_quit == NULL)) {
		 //create failed
		error_choice = ErrorBox(2,1);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			FreeSurfaceWrap(&surf_new);
			FreeSurfaceWrap(&surf_load);
			FreeSurfaceWrap(&surf_quit);
			DestroyTextureWrap(tex_new);
			DestroyTextureWrap(tex_load);
			DestroyTextureWrap(tex_quit);
			SDL_DestroyRenderer(rend);
			SDL_DestroyWindow(*window);
			SDL_Quit();
			return 0;
		}
		if(error_choice == 3){ //ignore
			break;
		}
		if(tex_new == NULL){
			tex_new = CreateTextureWrap(rend,surf_new);
		}
		if(tex_load == NULL){
			tex_load = CreateTextureWrap(rend,surf_load);
		}
		if(tex_quit == NULL){
			tex_quit = CreateTextureWrap(rend,surf_quit);
		}
	}

	FreeSurfaceWrap(&surf_new);
	FreeSurfaceWrap(&surf_load);
	FreeSurfaceWrap(&surf_quit);

	SDL_Rect rect_new = { .x = 200, .y = 125, .w = 150, .h = 75 };
	SDL_Rect rect_load = { .x = 200, .y = 250, .w = 150, .h = 75 };
	SDL_Rect rect_quit = { .x = 200, .y = 375, .w = 150, .h = 75 };

	// event handling loop
	int done = -1;
	SDL_Event e;

	while (done == -1) {

		while (SDL_PollEvent(&e)) {
			if(e.button.type == SDL_MOUSEBUTTONDOWN){
				if((inRect(e.button.x,e.button.y,rect_new) == 1)){
					done=2;  //new game value
				}
				if((inRect(e.button.x,e.button.y,rect_load) == 1)){
					done=4; //load game value
				}
				if((inRect(e.button.x,e.button.y,rect_quit) == 1)){
					done=0; //quit value
				}
			}
			if(e.type==SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_CLOSE){
				done =0; //quit (by the x button)
			}
		}

		// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
		SDL_RenderClear(rend);

		// draw buttons
		SDL_RenderCopy(rend,tex_new,NULL,&rect_new);
		SDL_RenderCopy(rend,tex_load,NULL,&rect_load);
		SDL_RenderCopy(rend,tex_quit,NULL,&rect_quit);

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}

	// free function only objects and clear the renderer
	SDL_DestroyTexture(tex_new);
	SDL_DestroyTexture(tex_load);
	SDL_DestroyTexture(tex_quit);
	SDL_DestroyRenderer(rend);

	return done;
}

int chGUISettingWindow(SDL_Window* window,CHGame** src){
	if((window == NULL)||(src == NULL)){
			printf("%schGUIsettingWindow",ERR_NULL);
			SDL_Quit();
			return 0;
		}

		// create a renderer for the window
			SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	int i,j,error_choice;

		while (rend == NULL) {
			printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
			error_choice = ErrorBox(1,0);
			if(error_choice == 1){
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
			rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
		}

	char* set_filename[14] = {"GameMode","UserColor","1Player","2Player","White","Black","amateur","easy","moderate"
	,"hard","expert","Difficulty","Start","Back"};
	SDL_SetWindowTitle(window,"Settings");
	SDL_SetWindowSize(window,600,600);
	SDL_Surface* surf_set[14];

	char fullpath[300]; //i assume a path shorter than 200 characters. unless someone changes the code it is always true.

	//load images
	for(i=0;i<14;i++){
		snprintf(fullpath, sizeof fullpath, "%s%s%s", IMAGE_PATH_START,set_filename[i],IMAGE_PATH_END);
		surf_set[i] = LoadBMPWrap(fullpath);

		while(surf_set[i] == NULL){//LoadBMP failed
			error_choice = ErrorBox(0,1);
			if(error_choice == 2){ // try again
				surf_set[i] = LoadBMPWrap(fullpath);
			}
			else{ //quit or ignore
				if(error_choice == 3){ //ignore
					break;
				}
				for(j = 0; j<i;j++){ //free already loaded surfaces
					FreeSurfaceWrap(&surf_set[j]);
				}
				SDL_DestroyRenderer(rend);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
		}
	}

	//create textures
	SDL_Texture* tex_set[14];
	for(i = 0;i < 14;i++){
			tex_set[i] = CreateTextureWrap(rend,surf_set[i]);
			while(tex_set[i] == NULL){ //create texture failed
				error_choice = ErrorBox(2,1);
				if(error_choice== 2){ // try again
					tex_set[i] = CreateTextureWrap(rend,surf_set[i]);
				}
				else{ //quit or ignore
					if(error_choice == 3){ //ignore
						break;
					}
					for(j = 0; j<14;j++){ //free already created texutres and surfaces
						DestroyTextureWrap(tex_set[j]);
						FreeSurfaceWrap(&surf_set[j]);
					}
					SDL_DestroyRenderer(rend);
					SDL_DestroyWindow(window);
					SDL_Quit();
					return 0;
				}
			}
			//texure careated sucessfully
			FreeSurfaceWrap(&surf_set[i]);
	}


	SDL_Rect rect_mode = { .x = 50, .y = 50, .w = 100, .h = 50 };
	SDL_Rect rect_1p = { .x = 150, .y = 50, .w = 100, .h = 50 };
	SDL_Rect rect_2p = { .x = 350, .y = 50, .w = 100, .h = 50 };
	SDL_Rect rect_color = { .x = 50, .y = 125, .w = 100, .h = 50 };
	SDL_Rect rect_white = { .x = 150, .y = 125, .w = 100, .h = 50 };
	SDL_Rect rect_black = { .x = 350, .y = 125, .w = 100, .h = 50 };
	SDL_Rect rect_diff = { .x = 200, .y = 200, .w = 100, .h = 50 };
	SDL_Rect rect_diff1 = { .x = 25, .y = 300, .w = 100, .h = 50 };
	SDL_Rect rect_diff2 = { .x = 125, .y = 300, .w = 100, .h = 50 };
	SDL_Rect rect_diff3 = { .x = 225, .y = 300, .w = 100, .h = 50 };
	SDL_Rect rect_diff4 = { .x = 325, .y = 300, .w = 100, .h = 50 };
	SDL_Rect rect_diff5 = { .x = 425, .y = 300, .w = 100, .h = 50 };
	SDL_Rect rect_start = { .x = 50, .y = 500, .w = 100, .h = 50 };
	SDL_Rect rect_back = { .x = 500, .y = 500, .w = 100, .h = 50 };


	int done = -1;
	SDL_Event e;

	// event handling loop
	while (done == -1) {
		while (SDL_PollEvent(&e)) {
			if(e.button.type == SDL_MOUSEBUTTONDOWN){ //would have been much better to skip all the "if"s once you found the right rect. can't find
				//a better method though. (the buttons from the widget example arn't receiving any arguments so i rather used what i already know).
				if(inRect(e.button.x,e.button.y,rect_white) == 1){ //white
					(*src)->humancolor=1;
				}
				if(inRect(e.button.x,e.button.y,rect_black) == 1){ //black
					(*src)->humancolor=0;
				}
				if(inRect(e.button.x,e.button.y,rect_1p) == 1){ //1-player
					(*src)->humanplayer=1;
				}
				if(inRect(e.button.x,e.button.y,rect_2p) == 1){ //2 players
					(*src)->humanplayer=2;
				}
				if(inRect(e.button.x,e.button.y,rect_diff1) == 1){ //amateur
					(*src)->diff=1;
				}
				if(inRect(e.button.x,e.button.y,rect_diff2) == 1){ //easy
					(*src)->diff=2;
				}
				if(inRect(e.button.x,e.button.y,rect_diff3) == 1){ //moderate
					(*src)->diff=3;
				}
				if(inRect(e.button.x,e.button.y,rect_diff4) == 1){ //hard
					(*src)->diff=4;
				}
				if(inRect(e.button.x,e.button.y,rect_diff5) == 1){ //expert
					(*src)->diff=5;
				}
				if(inRect(e.button.x,e.button.y,rect_start) == 1){ //start
					done=3;
				}
				if(inRect(e.button.x,e.button.y,rect_back) == 1){  //back
					done=1;
				}
			}
			if(e.type==SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_CLOSE){
				done =0; //quit (by the x button)
			}
		}

		// clear window to color red (r,g,b,a)
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
		SDL_RenderClear(rend);

		// draw our window
		SDL_SetRenderDrawColor(rend, 0, 0, 0,0);
		SDL_RenderCopy(rend,tex_set[0],NULL,&rect_mode);
		SDL_RenderCopy(rend,tex_set[2],NULL,&rect_1p);
		SDL_RenderCopy(rend,tex_set[3],NULL,&rect_2p);
		SDL_RenderCopy(rend,tex_set[12],NULL,&rect_start);
		SDL_RenderCopy(rend,tex_set[13],NULL,&rect_back);

		if((*src)->humanplayer == 1){ //1 player mode. showing player color and difficulty options
			SDL_RenderCopy(rend,tex_set[1],NULL,&rect_color);
			SDL_RenderCopy(rend,tex_set[4],NULL,&rect_white);
			SDL_RenderCopy(rend,tex_set[5],NULL,&rect_black);
			SDL_RenderCopy(rend,tex_set[11],NULL,&rect_diff);
			SDL_RenderCopy(rend,tex_set[6],NULL,&rect_diff1);
			SDL_RenderCopy(rend,tex_set[7],NULL,&rect_diff2);
			SDL_RenderCopy(rend,tex_set[8],NULL,&rect_diff3);
			SDL_RenderCopy(rend,tex_set[9],NULL,&rect_diff4);
			SDL_RenderCopy(rend,tex_set[10],NULL,&rect_diff5);
			SDL_RenderDrawRect(rend,&rect_1p);
			if((*src)->humancolor == 1){
				SDL_RenderDrawRect(rend,&rect_white);
			}
			else{
				SDL_RenderDrawRect(rend,&rect_black);
			}
			switch((*src)->diff){
			case 1:
				SDL_RenderDrawRect(rend,&rect_diff1);
				break;
			case 2:
				SDL_RenderDrawRect(rend,&rect_diff2);
				break;
			case 3:
				SDL_RenderDrawRect(rend,&rect_diff3);
				break;
			case 4:
				SDL_RenderDrawRect(rend,&rect_diff4);
				break;
			case 5:
				SDL_RenderDrawRect(rend,&rect_diff5);
				break;
			}
		}
		else{  //2 player mode
			SDL_RenderDrawRect(rend,&rect_2p);
		}



		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}

	// free function only objects and clear the renderer
	for(i=0;i<14;i++){
		DestroyTextureWrap(tex_set[i]);
	}
	SDL_DestroyRenderer(rend);
	return done;
}

int chGuiGameWindow(SDL_Window* window,CHGame** src){
	if((window == NULL)||(src == NULL)){
		printf("%schGUIGameWindow\n",ERR_NULL);
		SDL_Quit();
		return 0;
	}

	SDL_SetWindowTitle(window,"Game");
	SDL_SetWindowSize(window,800,600);
	int i,j,error_choice;
	error_choice = 0;


	// create a renderer for the window
		SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
		while (rend == NULL) {
				printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
				error_choice = ErrorBox(1,0);
				if(error_choice == 1){
					SDL_DestroyWindow(window);
					SDL_Quit();
					return 0;
				}
				rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
		}

	char* options_filename[9] = {"RestartGame","LoadGame","SaveGame","MainMenu","Undo","Quit","K_checked","kchecked","Saved"};
	SDL_Surface* surf_options[9];

	char fullpath[300]; //i assume a path shorter than 200 characters. unless someone changes the code it is always true.

	//load option images
	for(i=0;i<9;i++){
		snprintf(fullpath, sizeof fullpath, "%s%s%s", IMAGE_PATH_START,options_filename[i],IMAGE_PATH_END);
		surf_options[i] = LoadBMPWrap(fullpath);
		while(surf_options[i] == NULL){ //LoadBMP failed
			error_choice = ErrorBox(0,1);
			if(error_choice == 2){ //try again
				surf_options[i] = LoadBMPWrap(fullpath);
			}
			else{//ignore or quit
				if(error_choice == 3){ //ignore
					break;
				}
				for(j = 0; j<i;j++){
					FreeSurfaceWrap(&surf_options[j]);
				}
				SDL_DestroyRenderer(rend);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
		}
	}

	char* pieces_filename[13]={"Board","k_","q_","b_","n_","r_","m_","K","Q","R","N","M","B"};
	SDL_Surface* surf_pieces[13];

	//load piece images
	for(i=0;i<13;i++){
		snprintf(fullpath, sizeof fullpath, "%s%s%s", PIECE_PATH_START,pieces_filename[i],IMAGE_PATH_END);
		surf_pieces[i] = LoadBMPWrap(fullpath);
		while(surf_pieces[i] == NULL){ //LoadBMP failed
			error_choice = ErrorBox(0,1);
			if(error_choice == 2){
				surf_pieces[i] = LoadBMPWrap(fullpath);
			}
			else{ //ignore or quit
				if(error_choice == 3){ // ignore
					break;
				}
				for(j = 0; j<i;j++){
					FreeSurfaceWrap(&surf_pieces[j]);
				}
				for(j =0; j < 9; j++){
					FreeSurfaceWrap(&surf_options[j]);
				}
				SDL_DestroyRenderer(rend);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}
		}
	}


	// set a specific color (gray: 127,127,127) to transparent
	for(i =1; i < 13;i++){
		if(surf_pieces[i] != NULL){ //makes sure the surface loaded successfully (otherwise the program crashes)
			SDL_SetColorKey(surf_pieces[i], SDL_TRUE, SDL_MapRGB(surf_pieces[i]->format, 127, 127, 127));
		}
	}

	//create textures
		SDL_Texture* tex_options[9];
		SDL_Texture* tex_pieces[13];
		for(i = 0;i < 13;i++){
			if(i < 9){
				tex_options[i] = CreateTextureWrap(rend,surf_options[i]);
				while(tex_options[i] == NULL){ //create texture failed
					error_choice = ErrorBox(2,1);
					if(error_choice == 2){ //try again
						tex_options[i] = CreateTextureWrap(rend,surf_options[i]);
					}
					else{ //ignore or quit
						if(error_choice == 3){ //ignore
							break;
						}
						for(i =0; i < 13; i++){
							if(i < 9){
								DestroyTextureWrap(tex_options[i]);
								FreeSurfaceWrap(&surf_options[i]);
							}
							DestroyTextureWrap(tex_pieces[i]);
							FreeSurfaceWrap(&surf_pieces[i]);
						}
						SDL_DestroyRenderer(rend);
						SDL_DestroyWindow(window);
						SDL_Quit();
						return 0;
					}
				}
				FreeSurfaceWrap(&surf_options[i]);
			}
			tex_pieces[i] = CreateTextureWrap(rend,surf_pieces[i]);

			while(tex_pieces[i] == NULL){ //create texture failed
				error_choice = ErrorBox(2,1);
				if(error_choice == 2){ //try again
					tex_pieces[i] = CreateTextureWrap(rend,surf_pieces[i]);
				}
				else{ //ignore or quit
					if(error_choice == 3){ //ignore
						break;
					}
					for(i =0; i < 13; i++){
						if(i < 9){
							DestroyTextureWrap(tex_options[i]);
							FreeSurfaceWrap(&surf_options[i]);
						}
						DestroyTextureWrap(tex_pieces[i]);
						FreeSurfaceWrap(&surf_pieces[i]);
					}
					SDL_DestroyRenderer(rend);
					SDL_DestroyWindow(window);
					SDL_Quit();
					return 0;
				}
			}
			FreeSurfaceWrap(&surf_pieces[i]);
		}


		SDL_Rect rect_board = { .x = 0, .y = 0, .w = 600, .h = 600 };
		SDL_Rect rect_pos[CH_GAME_N_ROWS*CH_GAME_N_COLUMNS] ;  //game piece positions
		SDL_Rect rect_options[9] ;  //option buttons and status notification positions


		for(i=0; i< CH_GAME_N_ROWS*CH_GAME_N_COLUMNS; i++ ){
			rect_pos[i].h=600/8;
			rect_pos[i].w=600/8;
			rect_pos[i].x= (i%CH_GAME_N_COLUMNS)*rect_pos[i].w-2;
			rect_pos[i].y= 530-(i/CH_GAME_N_COLUMNS)*rect_pos[i].h-6;
		}

		for(i=0; i<9;i++){
			rect_options[i].h=80;
			rect_options[i].w=80;
			rect_options[i].x=605;
			rect_options[i].y=(600/6)*i;
			if(i >= 3){
				rect_options[i].x =rect_options[i].x+85;
				rect_options[i].y =rect_options[i].y-300;
			}
			if(i >= 6){
				rect_options[i].x =rect_options[i].x-85;
				rect_options[i].w=180;
				rect_options[i].h=80;
			}

		}

		// event handling loop
		int done = -1;
		int clicked_first=-1;  //the first left click
		int click_old=-1; //remember the last get_moves position (so the program won't check the same move hundreds of times)
		int clicked_2nd =-1;  //the 2nd left click.
		int click_temp=-1;
		int click_right=-1;
		int PcMove[2]= {-1,-1};  //holds the suggested move for the computer (if there is one)
		int check = 1; //as in check function result, not chess checkmate;
		int isSaved = 0;  //indicates that the current game state is saved.
		char isOver = 'N';   // indicates that the current game is over. 'N' - not over 'k' - white won 'K'-black won '_' - tie
		CHMoveList movs; //move list for get_moves
		SDL_Event e;
		while (done==-1) {
			switch(isOver){  //always check if the game is over before checking user's input
				case CH_PLAYER_1_KING:
					GameEndBox(0);
					done = 1;
					break;
				case CH_PLAYER_1_KING+'A'-'a':
					GameEndBox(1);
					done = 1;
					break;
				case CH_GAME_EMPTY_ENTRY:
					GameEndBox(2);
					done = 1;
					break;
				case 0: //an error occured
					while(isOver == 0){
						error_choice = ErrorBox(6,1);
						if(error_choice == 2){//try again
							isOver = chGameCheckWinner(*src);
						}
						else{ //quit or ignore
							if(error_choice == 3){ //ignore
								break;
							}
							done = 0;
						}
					}
					break;
				default:
					break;
			}

			while (SDL_PollEvent(&e)) {
				if((e.button.type == SDL_MOUSEBUTTONDOWN)){
					if(e.button.x<600){ //the click was on the game board and not on the options
						click_temp = e.button.x/rect_pos[0].w+CH_GAME_N_COLUMNS*((int)(600-e.button.y)/rect_pos[0].h);

						//right mouse button game board event
						if(e.button.button == SDL_BUTTON_RIGHT){
							if(click_right == click_temp){
								click_right = -1;
							}
							else{
							click_right=click_temp;
								if(click_right != click_old){
									//free(movs);
									movs=chGameGetMoves(*src,click_right);
									click_old = click_right;
									while(movs.listSize < 0){ //an error occured
										error_choice = ErrorBox(6,1);
										if(error_choice == 2){//try again
											movs=chGameGetMoves(*src,click_right);
										}
										else{ //quit or ignore
											if(error_choice == 3){ //ignore (will ignore the right click)
												click_right = -1;
												click_old = click_right;
												break;
											}
											done = 0;
										}
									}
								}
							}
						}

						//left mouse button game board events
						if(e.button.button == SDL_BUTTON_LEFT){
							if(click_temp == clicked_first){
								clicked_first=-1;
							}
							else{
								if(clicked_first != -1){
									//a 2nd click registered. make the move if valid.
									clicked_2nd=click_temp;
									do{
										switch(chGameIsValidMove(*src,clicked_first,clicked_2nd)){
										case CH_GAME_SUCCESS:
											error_choice = 0;
											check = 1; //no errors and valid move
											break;
										case CH_GAME_INVALID_ARGUMENT:
											check = -1; //an error occured
											break;
										default:
											check =0; //no error and invalid move
										}
										if(check == -1){ //handle error
											error_choice = ErrorBox(6,1);
											if(error_choice == 1){ // quit
												done=0;
												break;
											}
											if(error_choice == 3){ //ignore
												break;
											}
											error_choice = -1; // try again
										}
									}while(error_choice ==-1);
									if(check == 1){//the move is valid
										while(chGameSetMove(*src,clicked_first,clicked_2nd) != CH_GAME_SUCCESS){ //an error occurred
											error_choice = ErrorBox(6,1);
											if(error_choice == 1){//quit
												check = -1;
												done = 0;
												break;
											}
											if(error_choice == 3){ //ignore (should just ignore the move attempt and allow the player to try another move)
												check = -1;
												break;
											}
										}
										isOver = chGameCheckWinner(*src);
										click_old=-1; //piece was moved so the old movs set isn't valid
										click_right = -1;
										isSaved = 0; //the game changes so it isn't saved.


									}
								}
								else{
									if((chGameGetColor(*src,click_temp) == (*src)->currentPlayer)){
										clicked_first=click_temp;
									}
								}
							}
						}
					}
					else{ //the click was in the options area

						if(inRect(e.button.x,e.button.y,rect_options[0])==1){
							if(isSaved != 1){
								switch(SaveBox()){
								case 3: //yes
									chGuiSaveLoadWindow(0,src);
									chGameStarting(*src);  //restart
									break;
								case 2: //no
									chGameStarting(*src);  //restart
									break;
								case 1: //cancel
									break;
								}
							}
							else{
								chGameStarting(*src);  //restart
							}
							isSaved = 0;

						}
						if(inRect(e.button.x,e.button.y,rect_options[2])==1){
							if(chGuiSaveLoadWindow(0,src) == 1){ //save successful
								isSaved = 1;
							}

						}

						if(inRect(e.button.x,e.button.y,rect_options[1])==1){
							switch(chGuiSaveLoadWindow(1,src)){
							case 0: //load unsuccessful
								//error massage was moved to the save/load window
								break;
							case 2: //pressed the "back" button
								break;
							case 1: //loaded successful
								isSaved = 1;
								break;
							default:
								break;
							}
						}
						if(inRect(e.button.x,e.button.y,rect_options[4])==1){ //undo
							while(chGameUndoPrevMove(*src) == CH_GAME_INVALID_ARGUMENT){
								//error occurred
								error_choice = ErrorBox(6,1);
								if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
									done = 0;
									break;
								}
								if(error_choice == 3){ //ignore (should ignore the undo attempt and allow the player to keep playing)
									break;
								}

							}
							if(((*src)->humanplayer == 1)&&((*src)->move_list->actualSize > 0)){ //only one human player and move history not empty
								while(chGameUndoPrevMove(*src) == CH_GAME_INVALID_ARGUMENT){
									//error occurred
									error_choice = ErrorBox(6,1);
									if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
										done = 0;
										break;
									}
									if(error_choice == 3){ //ignore
										break;
									}

								}
							}
						}
						if(inRect(e.button.x,e.button.y,rect_options[3])==1){ //main menu
							if(isSaved != 1){
								switch(SaveBox()){
								case 3: //yes
									chGuiSaveLoadWindow(0,src);
									chGameDestroy(*src);
									done=1; //main menu
									break;
								case 2: //no
									chGameDestroy(*src);
									done=1; //main menu
									break;
								case 1: //cancel
									break;
								}

							}
							else{
								chGameDestroy(*src);
								done=1; //main menu
							}


						}
						if(inRect(e.button.x,e.button.y,rect_options[5])==1){ //quit
							if(isSaved != 1){
								switch(SaveBox()){
								case 3: //yes
									chGuiSaveLoadWindow(0,src);
									done=0;  //quit
									break;
								case 2: //no
									done=0;  //quit
									break;
								case 1: //cancel
									break;
								}

							}
							else{
								done =0 ;
							}
						}
					}
				} //end of mousebutton event
				if(e.type==SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_CLOSE){
					done =0; //quiit (by the x button)
				}
			}

			//computer's turn (if there is one)
			if((chGameIsHumanTurn(*src) == false)&&(isOver == 'N')){ //pc's turn and the game is not over.
				check = chMinimaxSuggestMove(*src,(*src)->diff,&PcMove[0],&PcMove[1]) ;
				while(check < 1){ //an error occurred
					if(check < 0){ //computer still calculated a move despite the error
						error_choice = ErrorBox(7,1);
					}
					else{
						error_choice = ErrorBox(7,0); //computer didn't get a suggested move, can't ignore it.
					}
						if(error_choice == 2){ //try again
							check = chMinimaxSuggestMove(*src,(*src)->diff,&PcMove[0],&PcMove[1]);
						}
						else{
							if(error_choice == 3){ //ignore
								check = 1;
								break;
							}
							done = 0; // quit
							break;
						}
				}

				if(check == 1){ //computer found a move without errors
					while( chGameSetMove(*src, PcMove[0],PcMove[1]) == CH_GAME_INVALID_ARGUMENT){
						//error occurred *if SetMove failed the game board won't change.
						error_choice = ErrorBox(6,0);
						if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
							done = 0;
							break;
						}
					}
				}
				isOver = chGameCheckWinner(*src);
			}
			click_temp=-1;

			// clear window to color red (r,g,b,a)
			SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
			SDL_RenderClear(rend);

			// draw our window
			SDL_SetRenderDrawColor(rend, 0, 0, 0,0);
			SDL_RenderCopy(rend,tex_pieces[0],NULL,&rect_board);
			char piece;
			int pos;
			int color=0;
			int chk=0;
			int i,j;

			// ensure renderer supports transparency
			SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
			SDL_SetRenderDrawColor(rend, 0, 55, 0,0);
			if(clicked_first != -1){

				SDL_RenderFillRect(rend,&rect_pos[clicked_first]);
			}

			//drawing get_moves if needed
			if(click_right != -1){
				if(click_right != clicked_first){ //in case we want to see the moves of the left clicked piece. the rect will be dark green (same as left clicked)
					SDL_SetRenderDrawColor(rend, 160, 160, 220,0);
					SDL_RenderFillRect(rend,&rect_pos[click_right]);
				}
				//draw all possible moves for click_right
				for(i=0; i < movs.listSize;i++){
					for(j=0;j<CH_GAME_N_ROWS*CH_GAME_N_COLUMNS;j++){

						if((CHThreatMapCheck((*src)->threatMap,movs.moveList[i], j) == true)&&(chGameGetColor(*src,j) != chGameGetColor(*src,click_right))){
							chk=1;
							break;
						}
					}
					if(chk == 1){  //target position is threatened
						if(chGameGetColor((*src),movs.moveList[i]) != 2){  //target position isn't empty (attack)
							SDL_SetRenderDrawColor(rend, 139,0, 0,0); //dark red
						}
						else{ //movement into threatened
							SDL_SetRenderDrawColor(rend, 180, 140, 0,0); //dark yellow
						}
					}
					else{ //target position isn't threatened
						if(chGameGetColor((*src),movs.moveList[i]) != 2){  //target position isn't empty (attack)
							SDL_SetRenderDrawColor(rend, 0,150, 0,0); //bright green
						}
						else{ //movement
							SDL_SetRenderDrawColor(rend, 0, 0, 150,0);  //blue
						}
					}
					SDL_RenderFillRect(rend,&rect_pos[movs.moveList[i]]);
					chk=0;
				}
			}

			//rendering option buttons
			for(i = 0; i < 6; i++){
				SDL_RenderCopy(rend,tex_options[i],NULL,&rect_options[i]);
			}

			SDL_SetRenderDrawColor(rend, 0, 0, 0,0);

			if(isSaved == 1){ //current game is saved
				SDL_RenderCopy(rend,tex_options[8],NULL,&rect_options[8]);
				SDL_RenderDrawRect(rend,&rect_options[8]);
			}
			if((*src)->isCheck == true){
				if((*src)->currentPlayer == 0){ //it's the black player's turn and there is a check (that means black king is checked)
					SDL_RenderCopy(rend,tex_options[6],NULL,&rect_options[6]);
					SDL_RenderDrawRect(rend,&rect_options[6]);
				}
				else{
					SDL_RenderCopy(rend,tex_options[7],NULL,&rect_options[7]);
					SDL_RenderDrawRect(rend,&rect_options[7]);
				}
			}

			SDL_RenderDrawRects(rend,rect_options,6);


			// ensure renderer supports transparency
			SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

			//rendering the game pieces
			for(i=0;i<CH_GAME_N_COLUMNS;i++){
				for(j=0;j<CH_GAME_N_ROWS;j++){
					color = 1;
					piece = (*src)->gameBoard[i][j];
					pos = chGamePos(i,j);
					if((piece>='A')&&(piece <= 'Z')){
						color = 0;
						piece+='a'-'A';
					}
					switch(piece){
					case CH_PLAYER_1_PAWN:
						if(color == 1){
							SDL_RenderCopy(rend,tex_pieces[6],NULL,&rect_pos[pos]);
						}
						else{
							SDL_RenderCopy(rend,tex_pieces[11],NULL,&rect_pos[pos]);
						}
						break;
					case CH_PLAYER_1_KING:
						if(color == 1){
							SDL_RenderCopy(rend,tex_pieces[1],NULL,&rect_pos[pos]);
						}
						else{
							SDL_RenderCopy(rend,tex_pieces[7],NULL,&rect_pos[pos]);
						}
						break;
					case CH_PLAYER_1_QUEEN:
						if(color == 1){
							SDL_RenderCopy(rend,tex_pieces[2],NULL,&rect_pos[pos]);
						}
						else{
							SDL_RenderCopy(rend,tex_pieces[8],NULL,&rect_pos[pos]);
						}
						break;
					case CH_PLAYER_1_ROOK:
						if(color == 1){
							SDL_RenderCopy(rend,tex_pieces[5],NULL,&rect_pos[pos]);
						}
						else{
							SDL_RenderCopy(rend,tex_pieces[9],NULL,&rect_pos[pos]);
							}
						break;
					case CH_PLAYER_1_KNIGHT:
						if(color == 1){
							SDL_RenderCopy(rend,tex_pieces[4],NULL,&rect_pos[pos]);
						}
						else{
							SDL_RenderCopy(rend,tex_pieces[10],NULL,&rect_pos[pos]);
						}
						break;
					case CH_PLAYER_1_BISHOP:
						if(color == 1){
							SDL_RenderCopy(rend,tex_pieces[3],NULL,&rect_pos[pos]);
						}
						else{
							SDL_RenderCopy(rend,tex_pieces[12],NULL,&rect_pos[pos]);
						}
						break;
					}

					if(clicked_2nd !=-1){
						clicked_first=-1;
						clicked_2nd =-1;
					}
				}
			}


			// present changes to user
			SDL_RenderPresent(rend);

			// small delay
			SDL_Delay(10);
		}

		// free function only objects, the options window and clear the renderer
		for(i=0; i < 13;i++){
			if(i < 9){
				DestroyTextureWrap(tex_options[i]);
			}
			DestroyTextureWrap(tex_pieces[i]);
		}

		SDL_DestroyRenderer(rend);
		return done;
}

int chGuiSaveLoadWindow(int isLoad,CHGame** src){
	if(src == NULL){
		printf("%schGUiSaveLoadWindw",ERR_NULL);
		return 0;
	}

	int error_choice;
	const char* window1_title;
	if(isLoad == 1){
		window1_title = "Load menu";
	}
	else{
		window1_title = "Save menu";
	}

	// create an SDL window1
	SDL_Window* window1 = SDL_CreateWindow(
		window1_title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		400,
		400,
		SDL_WINDOW_SHOWN);

	// make sure window1 was created successfully
	while(window1 == NULL){ //window didn't start correctly
		error_choice = ErrorBox(4,0);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			return 0;
		}
		window1 = SDL_CreateWindow(
				"Main menu",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				600,
				600,
				SDL_WINDOW_SHOWN); //using SDL_WINDOW_SHOWN according to the forum

	}

	// create a renderer for the window1
	SDL_Renderer* rend = SDL_CreateRenderer(window1, -1, SDL_RENDERER_SOFTWARE);
	while (rend == NULL) {
		printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
		error_choice = ErrorBox(1,0);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			SDL_DestroyWindow(window1);
			SDL_Quit();
			return 0;
		}
		rend = SDL_CreateRenderer(window1, -1, SDL_RENDERER_SOFTWARE);
	}

	int slot[SAVE_SLOTS+5]={0}; //indicator for whether the file exists or not.
	SDL_Surface* surf_save[SAVE_SLOTS];
	char* dest=SAVE_PATH;
	char* image_dest = IMAGE_PATH_START;
	char* image_dest_end = IMAGE_PATH_END;
	char fp[sizeof(SAVE_SLOTS)+sizeof(SAVE_PATH)];
	char image[sizeof(SAVE_SLOTS)+sizeof(IMAGE_PATH_START)+4];
	FILE* f;
	int i,j;

	//check for existing files
	for(i=0; i<SAVE_SLOTS;i++){
		snprintf(fp, sizeof fp, "%s%d",dest,i+1);
		f=fopen(fp,"r");
		if( f != NULL){ //the file exists
			slot[i] = 1;
			fclose(f);
		}

		//loading images
		snprintf(image, sizeof image, "%s%d%s", image_dest, i+1,image_dest_end);
		surf_save[i] =LoadBMPWrap(image);
		while(surf_save[i] == NULL){ //an error occurred
			error_choice = ErrorBox(0,1);
			if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
				for(j=0; j<i;j++){
					FreeSurfaceWrap(&surf_save[j]);
				}
				SDL_DestroyRenderer(rend);
				SDL_DestroyWindow(window1);
				return 0;
			}
			if(error_choice == 3){ //ignore
				break;
			}
			surf_save[i] =LoadBMPWrap(image);
		}
	}
	SDL_Surface* surf_back =LoadBMPWrap("./images/Options/Back.bmp");
	SDL_Surface* surf_up =LoadBMPWrap("./images/Options/UP.bmp");
	SDL_Surface* surf_down =LoadBMPWrap("./images/Options/DOWN.bmp");
	while((surf_back == NULL)||(surf_up == NULL)||(surf_down == NULL)) {
		error_choice = ErrorBox(0,1);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			for(i=0; i< SAVE_SLOTS; i++){
				FreeSurfaceWrap(&surf_save[i]);
			}
			FreeSurfaceWrap(&surf_down);
			FreeSurfaceWrap(&surf_up);
			FreeSurfaceWrap(&surf_back);
			SDL_DestroyRenderer(rend);
			SDL_DestroyWindow(window1);
			return 0;
		}
		if(error_choice == 3){ //ignore
			break;
		}
		if(surf_back == NULL){
			surf_back =LoadBMPWrap("./images/Options/Back.bmp");
		}
		if(surf_up == NULL){
			surf_up =LoadBMPWrap("./images/Options/UP.bmp");
		}
		if(surf_down== NULL){
			surf_down =LoadBMPWrap("./images/Options/DOWN.bmp");
		}
	}

	//creating textures
	SDL_Texture* tex_save[SAVE_SLOTS];

	for(i=0; i < SAVE_SLOTS; i++){
		tex_save[i] = CreateTextureWrap(rend,surf_save[i]);
		while(tex_save[i] == NULL){ //error - texture create failed
			error_choice = ErrorBox(2,1);
			if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
				for(j=0; j<SAVE_SLOTS;j++){
					DestroyTextureWrap(tex_save[j]);
				}
				FreeSurfaceWrap(&surf_down);
				FreeSurfaceWrap(&surf_up);
				FreeSurfaceWrap(&surf_back);
				SDL_DestroyRenderer(rend);
				SDL_DestroyWindow(window1);
				return 0;
			}
			if(error_choice == 3){ //ignore
				break;
			}
			tex_save[i] = CreateTextureWrap(rend,surf_save[i]);
		}
		FreeSurfaceWrap(&surf_save[i]);
	}


	SDL_Texture* tex_back = CreateTextureWrap(rend,surf_back);
	SDL_Texture* tex_up = CreateTextureWrap(rend,surf_up);
	SDL_Texture* tex_down = CreateTextureWrap(rend,surf_down);


	while((tex_back == NULL)||(tex_up == NULL)||(tex_down == NULL)) {
		//texture create failed
		error_choice = ErrorBox(2,1);
		if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
			for(i=0; i< SAVE_SLOTS; i++){
				DestroyTextureWrap(tex_save[i]);
			}
			DestroyTextureWrap(tex_back);
			DestroyTextureWrap(tex_up);
			DestroyTextureWrap(tex_down);
			SDL_DestroyRenderer(rend);
			SDL_DestroyWindow(window1);;
			return 0;
		}
		if(error_choice == 3){
			break;
		}
		if(tex_back == NULL){
			tex_back = CreateTextureWrap(rend,surf_back);
		}
		if(tex_up == NULL){
			tex_up = CreateTextureWrap(rend,surf_up);
		}
		if(tex_down== NULL){
			tex_down = CreateTextureWrap(rend,surf_down);
		}
	}

	FreeSurfaceWrap(&surf_back);
	FreeSurfaceWrap(&surf_up);
	FreeSurfaceWrap(&surf_down);

	SDL_Rect rect_save[5];
	for(i=0; i<5;i++){
		rect_save[i].w = 50;
		rect_save[i].h = 50;
		rect_save[i].x = 150;
		rect_save[i].y = 50 +i*50;
	}

	SDL_Rect rect_back = { .x = 0, .y = 350, .w = 100, .h = 50 };
	SDL_Rect rect_up = { .x = 350, .y = 25, .w = 50, .h = 150 };
	SDL_Rect rect_down = { .x = 350, .y = 175, .w = 50, .h = 150 };

	// event handling loop
	int done = -1;
	int page=1;
	SDL_Event e;
	while (done == -1) {
		while (SDL_PollEvent(&e)) {
			if(e.type==SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_CLOSE){
				done =2; //close the window (by the x button)
			}

			if(e.button.type == SDL_MOUSEBUTTONDOWN){
				if(inRect(e.button.x,e.button.y,rect_back) == 1){
					done=2;  //back to the previous window without loading
				}
				if(inRect(e.button.x,e.button.y,rect_up) == 1){
					if(page>1){  //up to previous page
						page--;
					}
				}
				if(inRect(e.button.x,e.button.y,rect_down) == 1){
					if(page*5 < SAVE_SLOTS ){//down to next page
						page++;
					}
				}
				for(i=0; i<5;i++){
					if(inRect(e.button.x,e.button.y,rect_save[i]) == 1){
						snprintf(fp, sizeof fp, "%s%d",dest,(page-1)*5+1+i);
						if(isLoad == 1){
							if(slot[(page-1)*5+i] == 1 ){//the file exists
								done = CHMainAuxLoad(src, fp);
								while(done == 0){ //error occured
									error_choice = ErrorBox(4,1);
									if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
										done = 2;
										break;
									}
									if(error_choice == 3){
										done = -1;
										break;
									}
									done = CHMainAuxLoad(src, fp);
								}
							}
						}
						else{
							done =CHMainAuxSave(*src, fp);
							while(done == 0){ //error occured
								error_choice = ErrorBox(5,1);
								if((error_choice == 1)||(error_choice == 0)){ //quit or error with the errorbox itself
									done = 2;
									break;
								}
								if(error_choice == 3){
									done = -1;
									break;
								}
								done =CHMainAuxSave(*src, fp);
							}
						}
					}
				}
			}
		}


		SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
		SDL_RenderClear(rend);

		// draw buttons
		SDL_RenderCopy(rend,tex_back,NULL,&rect_back);
		if(page > 1){ //we are not looking at the 1st page
			SDL_RenderCopy(rend,tex_up,NULL,&rect_up);
		}
		if((page-1)*5+5 < SAVE_SLOTS){ //there are still more possible save slots
			SDL_RenderCopy(rend,tex_down,NULL,&rect_down);
		}
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
		for(i=0; i<5;i++){
			if((slot[(page-1)*5+i] == 1)||((isLoad == 0)&&((page-1)*5+i < SAVE_SLOTS))){
				SDL_RenderCopy(rend,tex_save[(page-1)*5+i],NULL,&rect_save[i]);
				SDL_RenderDrawRect(rend,&rect_save[i]);
			}
		}

		// present changes to user
		SDL_RenderPresent(rend);

		// small delay
		SDL_Delay(10);
	}

	// free function only objects and clear the renderer
	for(i=0; i< SAVE_SLOTS;i++){
		SDL_DestroyTexture(tex_save[i]);
	}
	SDL_DestroyTexture(tex_back);
	SDL_DestroyTexture(tex_up);
	SDL_DestroyTexture(tex_down);
	SDL_RenderClear(rend);
	SDL_DestroyWindow(window1);

	return done;
}

//private functions implementations//

int inRect(int x, int y, SDL_Rect rect){ //return 0 if the point {x,y} is not in the rectangle. return 1 otherwise.
	if((x >= rect.x)&&(x <= rect.x+rect.w)&&(y >= rect.y)&&(y <= rect.y +rect.h)){
		return 1;
	}
	return 0;
}

//acording to SDL wiki it ISN'T safe to pass NULL to DestroyTexture (raises error). using a wrap function to check this.
void DestroyTextureWrap(SDL_Texture* tex){
	if(tex != NULL){
		SDL_DestroyTexture(tex);
	}
}

void FreeSurfaceWrap(SDL_Surface** surf){
	SDL_FreeSurface(*surf);
	*surf=NULL;
}
SDL_Texture* CreateTextureWrap(SDL_Renderer* rend, SDL_Surface* surf){
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend,surf);
	if(tex == NULL){
		printf("ERROR: unable to create texture: %s\n", SDL_GetError());
	}
	return tex;
}

SDL_Surface* LoadBMPWrap(const char* file){
	SDL_Surface* surf = SDL_LoadBMP(file);
	if(surf == NULL){
		printf("ERROR: %s\n", SDL_GetError());
	}
	return surf;
}


int ErrorBox(int err_num, int can_ignore){
	//most of the box code was copies right out of the example of sdl wiki.
	char* errors[]={"unable to load image", "unable to create renderer","unable to create texture",
			"unable to create window","coudln't load file","couldn't save file","an error occurred in game logic function",
			"an error occurred in min max algorithm.\n despite that, a move has been found\n. press ignore to continue\n"};
	 const SDL_MessageBoxButtonData buttons_canignore[] = {
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "quit" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "try again" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 3, "ignore" }
	    };
	 const SDL_MessageBoxButtonData buttons[] = {
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "quit" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "try again" },
	    };
	    const SDL_MessageBoxColorScheme colorScheme = {
	        { /* .colors (.r, .g, .b) */
	            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
	            { 255,   0,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
	            {   0, 255,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
	            { 255, 255,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
	            {   0,   0, 255 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
	            { 255,   0, 255 }
	        }
	    };
	    const SDL_MessageBoxData messageboxdata_can = {
	        SDL_MESSAGEBOX_INFORMATION, /* .flags */
	        NULL, /* .window */
	        "Error:", /* .title */
	        errors[err_num], /* .message */
	        SDL_arraysize(buttons_canignore), /* .numbuttons */
	        buttons, /* .buttons */
	        &colorScheme /* .colorScheme */
	    };
	    const SDL_MessageBoxData messageboxdata = {
	        SDL_MESSAGEBOX_INFORMATION, /* .flags */
	        NULL, /* .window */
	        "Error:", /* .title */
	        errors[err_num], /* .message */
	        SDL_arraysize(buttons), /* .numbuttons */
	        buttons, /* .buttons */
	        &colorScheme /* .colorScheme */
	    };
	    int buttonid;
	    if(can_ignore == 1){
			if(SDL_ShowMessageBox(&messageboxdata_can, &buttonid) < 0){
				printf("error displaying message box");
				return 0;
			}
			switch(buttonid){
				case 1: //quit
					return 1;
				case 2: //try again
					return 2;
				case 3: //ignore (try to keep running anyway, will most likely lead to a crash)
					return 3;
			  }
	    }
	    else{
			if(SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0){
				printf("error displaying message box");
				return 0;
			}
			switch(buttonid){
				case 1: //quit
					return 1;
				case 2: //try again
					return 2;
			  }
	    }
	    return 0;
}

int SaveBox(void){
	//most of the box code was copies right out of the example of sdl wiki.
	 const SDL_MessageBoxButtonData buttons[] = {
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yes" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "No" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "Cancel" },
	    };
	    const SDL_MessageBoxColorScheme colorScheme = {
	        { /* .colors (.r, .g, .b) */
	            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
	            { 255,   0,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
	            {   0, 255,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
	            { 255, 255,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
	            {   0,   0, 255 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
	            { 255,   0, 255 }
	        }
	    };
	    const SDL_MessageBoxData messageboxdata = {
	        SDL_MESSAGEBOX_INFORMATION, /* .flags */
	        NULL, /* .window */
	        "Game is not saved!", /* .title */
	        "do you want to save the current game?", /* .message */
	        SDL_arraysize(buttons), /* .numbuttons */
	        buttons, /* .buttons */
	        &colorScheme /* .colorScheme */
	    };
	    int buttonid;
	    if(SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0){
	        printf("error displaying message box");
	        return 0;
	    }
	    switch(buttonid){
	    case 0: //yes
	    	return 3;
	    case 1: //no
	    	return 2;
	    case 2: //cancel
	    	return 1;
	    }
	    return -1;
}

int GameEndBox(int i){
	//most of the box code was copies right out of the example of sdl wiki.
	 const SDL_MessageBoxButtonData buttons[] = {
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "ok" },
	    };
	    const SDL_MessageBoxColorScheme colorScheme = {
	        { /* .colors (.r, .g, .b) */
	            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
	            { 255,   0,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
	            {   0, 255,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
	            { 255, 255,   0 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
	            {   0,   0, 255 },
	            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
	            { 255,   0, 255 }
	        }
	    };
	    char* msg[3]={"white player won","black player won","game ended in a tie"};
	    const SDL_MessageBoxData messageboxdata = {
	        SDL_MESSAGEBOX_INFORMATION, /* .flags */
	        NULL, /* .window */
	        "Game Over!", /* .title */
	        msg[i], /* .message */
	        SDL_arraysize(buttons), /* .numbuttons */
	        buttons, /* .buttons */
	        &colorScheme /* .colorScheme */
	    };
	    int buttonid;
	    if(SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0){
	        printf("error displaying message box");
	        return 0;
	    }
	   return 1;
}
