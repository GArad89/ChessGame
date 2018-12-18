#ifndef CHPARSER_H_
#define CHPARSER_H_
#include <stdbool.h>

/**
 * function summary:
 *	chParserIsInt - Checks if a specified string represents a valid integer
 *	chParserPraseLineGame - Parses a specified line for the game phase.
 *	chParserPraseLineSetting - Parses a specified line for the game setting phase.
 *	chParserPraseLineSetting - Creates the save / load window.
 *	chParserLineError - parses the wanted response by the user in the case of the error in the console mode.
 */

//specify the maximum line length (according to the forum, we can assume a 2000 characters limit)
#define CH_MAX_LINE_LENGTH 2000

//a type used to represent a command in game
typedef enum {
	CH_UNDO_MOVE,
	CH_MOVE,
	CH_GET_MOVE,
	CH_QUIT,
	CH_RESTART,
	CH_INVALID_LINE,
	CH_SAVE,
} CH_COMMAND;

//a type used to represent a command in setting phase
typedef enum {
	CH_QUIT1,
	CH_INVALID_LINE1,
	CH_GAME_MODE,
	CH_DIFFICULTY,
	CH_USER_COLOR,
	CH_LOAD,
	CH_DEFAULT,
	CH_PRINT_SETTING,
	CH_START,
} CH_COMMAND_SET;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	CH_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg1,arg2;
	char* path;
} CHCommand;

//a new type that is used to encapsulate a parsed line
typedef struct command_s {
	CH_COMMAND_SET cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	char* path;
} CHCommandSet;

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool chParserIsInt(const char* str);

/**
 * Parses a specified line for the game phase. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true.
 * in case of a load or save command the argument is saved to "path" and isn't checked in this function.
 * In any other case then 'validArg' is set to false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
CHCommand chParserPraseLineGame(const char* str);

/**
 * Parses a specified line for the game setting phase. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. in case of a load command the argument is saved to "path" and isn't checked in this function.
 * In any other case then 'validArg' is set to false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
CHCommandSet chParserPraseLineSetting(const char* str);
/**
 *  parses the wanted response by the user in the case of the error in the console mode.
 *  the user can:
 *  'quit'- exit the program
 *  'try_again' -  attempt to call the failed function again and check if it was successful
 *
 *  in the case of any other input, the output value for quit will be sent.
 *  **note : the errors in the game were separated to 2 types:
 *  critical errors - errors that would crash the game and can't be ignored
 *  minor errors - errors that only send an error massage and don't ask for user's input.
 * (print functions, failure in one subnode of the minmax algorithm, or commands that can be
 *  Simply regarded as invalid command)
 *
 *  @return
 *  3 - try again
 *  1 - quit or otherwise
 */
int chParserLineError(void);
#endif
