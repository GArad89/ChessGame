#include <stdbool.h>
#include "CHParser.h"
#include "CHGame.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

bool chParserIsInt(const char* str){
	if(str == NULL){ //null pointer error
		printf("%schParserIsInt",ERR_NULL);
		return false;
	}
	int i=0;
	int first_char	=	0;  //set to 1 after the first non ' ' char
	int first_minus	=	0; //set to 1 after seeing the first '-' (for negative integers)
	while((str[i] != '\n')&&(str[i] != '\0')){

		if((str[i]	<	'0')||(str[i]	>	'9')){  //the char isn't a valid digit

			if(str[i]	==	'\0'){  //end of string
				return true;
			}

			if((str[i] != ' ')||(first_char == 1)){

				if((str[i] == '-')&&(first_minus == 0)){ //first '-' for a negative number
					first_minus	=	1;
				}
				else{
					return false;
				}
			}
			else{
				first_char	=	1;
			}
		}
		i++;
	}

	return true;
}


int chParserGetArg(const char* str){
	if(str == NULL){
		printf("%schParserGetArg",ERR_NULL);
		return false;
	}
	int i=1;
	int arg_value=0;
	if(str[0]!='<'){
		return -1; //-1 is an invalid position
	}
	while((str[i]	==	' ')||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
			i++;
	}
	if((str[i]<'1')||(str[i]>'1'+CH_GAME_N_COLUMNS-1)){
		return -1;
	}
	arg_value=(str[i]-'1')*CH_GAME_N_COLUMNS;
	i++;
	while((str[i]	==	' ')||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
			i++;
	}
	if(str[i]!=','){
		return -1;
	}
	i++;
	while((str[i]	==	' ')||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
			i++;
	}
	if((str[i]<'A')||(str[i]>'A'+CH_GAME_N_ROWS-1)){
		return -1;
	}
	arg_value+=str[i]-'A';
	i++;
	while((str[i]	==	' ')||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
			i++;
	}
	if(str[i]!='>'){
		return -1;
	}
	return arg_value; //i'm pretty sure we were told to ignore any additions after the position argument **NEED TO RECHECK AND MAKE SURE**
}

CHCommandSet chParserPraseLineSetting(const char* str){
	CHCommandSet command;
	if(str == NULL){
		printf("%schParseLineSetting",ERR_NULL);
		command.cmd = CH_INVALID_LINE1;
		command.arg = -1;
		return command;
	}

	command.validArg	=	false;
	char cmd[CH_MAX_LINE_LENGTH];
	int j;
	for(j	=	0; j	<	CH_MAX_LINE_LENGTH; j++){
		cmd[j]	=	'\0';  //initilize the array with nul characters
	}
	char poss_arg[CH_MAX_LINE_LENGTH];  //an array for a possible 2nd arguments
	for(j	=	0; j	<	CH_MAX_LINE_LENGTH; j++){
			poss_arg[j]	=	'\0';  //initilize the array with nul characters
		}
	char pos_cmd[8][20]={{"game_mode\0"},{"difficulty\0"},{"user_color\0"},{"load\0"},{"default\0"},{"print_settings\0"},{"quit\0"},{"start\0"}};
	int i	=	0;
	char blank	=	' ';


	while((str[i]	==	blank)||(str[i]	==	'	')){ //skips the whitespace before the input
		i++;
	}
	j	=	i;  //saving the starting index of the input
	while((str[i]	!=	blank)&&(str[i]	!=	'\n')&&(str[i]	!=	'	')&&(str[i]	!=	'\0')){  //write the possible command to the cmd array
		cmd[i-j]	=	str[i];
		i++;
	}

	while((str[i]	==	blank)||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
		i++;
	}
	j	=	i;  //saving the starting index of the possible integer.
	while((str[i]	!=	blank)&&(str[i]	!=	'\n')&&(str[i]	!=	'	')&&(str[i]	!=	'\0')){ //write the possible integer value or file path
		poss_arg[i-j]	=	str[i];

		i++;
	}

	switch(cmd[0]) {

		case 'g':
			if((strcmp(cmd,pos_cmd[0])	==	0)){
				command.cmd	=	CH_GAME_MODE;
				if((chParserIsInt(poss_arg)	==	true)){
					command.validArg	=	true;
					command.arg	=	atoi(poss_arg);
				}
				return command;
			}
			break;

		case 'q':
			if(strcmp(cmd,pos_cmd[6])	==	0){
				command.cmd	=	CH_QUIT1;
				command.validArg	=	false;
				return command;
			}
			break;



		case 'd':
			if((strcmp(cmd,pos_cmd[1])	==	0)){
				command.cmd	=	CH_DIFFICULTY;
				if((chParserIsInt(poss_arg)	==	true)){
					command.validArg	=	true;
					command.arg	=	atoi(poss_arg);
				}
				return command;
			}
			if(strcmp(cmd,pos_cmd[4])	==	0){
				command.cmd	=	CH_DEFAULT;
				command.validArg	=	false;
				return command;
			}
			break;

		case 'u':
			if((strcmp(cmd,pos_cmd[2])	==	0)){
				command.cmd	=	CH_USER_COLOR;
				if((chParserIsInt(poss_arg)	==	true)){
					command.validArg	=	true;
					command.arg	=	atoi(poss_arg);
				}
				return command;
			}
			break;
		case 'p':
					if((strcmp(cmd,pos_cmd[5])	==	0)){
						command.cmd	=	CH_PRINT_SETTING;
						command.validArg	=	false;
						return command;
					}
					break;
		case 's':
					if((strcmp(cmd,pos_cmd[7])	==	0)){
						command.cmd	=	CH_START;
						command.validArg	=	false;
						return command;
					}
					break;
		case 'l':
			if((strcmp(cmd,pos_cmd[3])	==	0)){
				command.cmd	=	CH_LOAD;
				command.validArg	=	true;
				command.path	=	(poss_arg);
				return command;
			}
	}
	command.cmd	=	CH_INVALID_LINE1;
	command.validArg	=	false;
	return command;
}

int chParserLineError(void){
	char str[CH_MAX_LINE_LENGTH];
	fgets(str,20,stdin);
	switch(str[0]){
	case 't':
		if(strcmp(str,"try_again\0") == 0){
			return 3;
		}
	}
	return 1; //quit
}

CHCommand chParserPraseLineGame(const char* str){
	CHCommand command;
	if(str == NULL){
		printf("%schParserPraseLineGame",ERR_NULL);
		command.cmd = CH_INVALID_LINE;
		command.arg1 = -1;
		return command;
	}
	char cmd[CH_MAX_LINE_LENGTH];
	char poss_arg1[CH_MAX_LINE_LENGTH],poss_arg2[CH_MAX_LINE_LENGTH];  // arrays for a possible arguments
	int j;
	for(j	=	0; j	<	CH_MAX_LINE_LENGTH; j++){
		cmd[j]	=	'\0';  //initilize the array with null characters
		poss_arg1[j]	=	'\0';  //same
		poss_arg2[j]	=	'\0';  //and same
	}

	char pos_cmd[6][20]={{"get_move\0"},{"undo\0"},{"move\0"},{"quit\0"},{"reset\0"},{"save\0"}};
	int i	=	0;
	char blank	=	' ';


	while((str[i]	==	blank)||(str[i]	==	'	')){ //skips the whitespace before the input
		i++;
	}
	j	=	i;  //saving the starting index of the input
	while((str[i]	!=	blank)&&(str[i]	!=	'\n')&&(str[i]	!=	'	')&&(str[i]	!=	'\0')){  //write the possible command to the cmd array
		cmd[i-j]	=	str[i];
		i++;
	}

	while((str[i]	==	blank)||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
		i++;
	}
	j	=	i;  //saving the starting index of the possible 1st argument
	while((str[i]	!=	blank)&&(str[i]	!=	'\n')&&(str[i]	!=	'	')&&(str[i]	!=	'\0')){ //write the possible integer value
		poss_arg1[i-j]	=	str[i];

		i++;
	}
	while((str[i]	==	blank)||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
		i++;
	}
	if(i<1999){ //makes sure we are still in the input string
		if((str[i]=='t')&&(str[i+1]=='o')){ //only case for a 2nd argument is a move command "move x to y"
			i+=2;
			while((str[i]	==	blank)||(str[i]	==	'	')){ //skip the whitespace before the possible integer value
				i++;
			}
			j=i;
			while((str[i]	!=	blank)&&(str[i]	!=	'\n')&&(str[i]	!=	'	')&&(str[i]	!=	'\0')){ //write the possible integer value
				poss_arg2[i-j]	=	str[i];
				i++;
			}
		}
	}
	switch(cmd[0]) {

		case 'r':
			if(strcmp(cmd,pos_cmd[4])	==	0){
				command.cmd	=	CH_RESTART;
				command.validArg	=	false;
				return command;
			}
			break;

		case 'q':
			if(strcmp(cmd,pos_cmd[3])	==	0){
				command.cmd	=	CH_QUIT;
				command.validArg	=	false;
				return command;
			}
			break;

		case 'm':
			if((strcmp(cmd,pos_cmd[2])	==	0)){
				command.cmd	=	CH_MOVE;
				command.arg1	=	chParserGetArg(poss_arg1);
				command.arg2	=	chParserGetArg(poss_arg2);
				if((command.arg1==-1)||(command.arg2==-1)){
					command.validArg=false;
				}
				else{
					command.validArg=true;
				}
				return command;
			}
			break;

		case 'u':
			if(strcmp(cmd,pos_cmd[1])	==	0){
				command.cmd	=	CH_UNDO_MOVE;
				command.validArg	=	false;
				return command;
			}
			break;

		case 'g':
			if(strcmp(cmd,pos_cmd[0])	==	0){
				command.cmd	=	CH_GET_MOVE;
				command.arg1 = chParserGetArg(poss_arg1);
				if(command.arg1 == -1){
					command.validArg	=	false;
				}
				else{
					command.validArg=true;
				}
				return command;
			}
			break;
		case 's':
			if(strcmp(cmd,pos_cmd[5])	==	0){
				command.cmd	=	CH_SAVE;
				command.validArg	=	true;
				command.path=poss_arg1;
				return command;
			}
			break;
	}
	command.cmd	=	CH_INVALID_LINE;
	command.validArg	=	false;
	return command;
}
