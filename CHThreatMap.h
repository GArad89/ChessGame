
#include <stdbool.h>

#ifndef CHTHREATMAP_H_
#define CHTHREATMAP_H_
/**
 * CHThreatMap Summary:
 *
 * contains the implementation of the game's threat map. the threat map is a bit array (implemented with int array).
 * if for example the piece in position 0 is threatened by position 23 then the 23th bit in the array will be 1. (0 otherwise)
 * vice verse (23 threatened by 0) will make the bit in position 23*CH_GAME_N_ROWS*CH_GAME_N_COLUMNS+0 be eqaul to 1.
 *
 * CHThreatMapCreate      - Creates a new empty threat map
 * CHThreatMapStarting    - Update the threat map to the starting position of a chess game
 * CHThreatMapCopy        - makes a copy of the threat map
 * CHThreatMapCheck       - returns true if pos is threatened by threat. return false otherwise
 * CHThreatMapUpdate      - update the corresponding pos-threat bit according to value (true =1, false =0)
 *
 */

/**
 * Initialize all the threat map values to 0;
 * @param
 * map - a pointer to an int array by the size: (int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1
 * the map array represents a bit array (although we use int)
 * @return
 * 1- on success
 * 0 - otherwise
 */
int CHThreatMapCreate(int* map);
/**
 * set the threat map to the start of a chess game.
 * @param
 * map - a pointer to an int array by the size: (int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1
 * the map array represents a bit array (although we use int)
 * @return
 * 1- on success
 * 0 - otherwise
 */
int CHThreatMapStarting(int* map);
/**
 * copy the map threat map to the cpy threat map.
 * @param
 * map - a pointer to an int array by the size: (int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1
 * the map array represents a bit array (although we use int)
 * cpy - a pointer to an int array by the size: (int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1
 * the map array represents a bit array (although we use int)
 * @return
 * 1- on success
 * 0 - otherwise
 */
int CHThreatMapCopy(int* src, int* cpy);
/**
 * checks if pos is threatened by threat according to the "map" threat map
 * @param
 * map - a pointer to an array by the size: (int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1
 * the map array represents a bit array (although we use int)
 * pos - a position value on the game board.
 * threat - a position value on the game board.
 * @return
 * true- if pos and threat aree valid positions and threat can attack pos on it's turn
 * false - otherwise
 */
bool CHThreatMapCheck(int* src,int pos, int threat);
/**
 * updates the "map" threat map, if pos is threatened by threat according to value.
 * @param
 * map - a pointer to an array by the size: (int)((CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)*(CH_GAME_N_ROWS*CH_GAME_N_COLUMNS)/32)+1
 * the map array represents a bit array (although we use int)
 * pos - a position value on the game board. values are in the range : [0... CH_GAME_N_ROWS*CH_GAME_N_COLUMNS]
 * threat - a position value on the game board. values are in the range : [0... CH_GAME_N_ROWS*CH_GAME_N_COLUMNS]
 * value - a boolen value. true if threat can now attack pos. false if threat no longer can attack pos.
 * @return
 * 1 - success
 * 0 - otherwise.
 */
int CHThreatMapUpdate(int* src,int pos, int threat,bool value);


#endif /* CHTHREATMAP_H_ */
