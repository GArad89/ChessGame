#ifndef CHARRAYLIST_H_
#define CHARRAYLIST_H_
#include <stdbool.h>
#include <stdio.h>

/**
 * CHArrayList summary:
 *
 * A container that represents a fixed size linked list. The capcity of the list
 * is specified at the creation. The container supports typical list
 * functionalities with the addition of random access as in arrays.
 * Upon insertion, if the maximum capacity is reached then an error message is
 * returned and the list is not affected.
 * an element is defined as a 3 cells integer subarray desricbing one movement.
 * first cell - the origin position of the moved piece
 * second cell - the target position of the moved piece
 * third cell - the char value of the eaten piece (or CH_GAME_EMPTY_TILE if none eaten)
 *
 * A summary of the supported functions is given below:
 *
 * chArrayListCreate       - Creates an empty array list with a specified
 *                           max capacity.
 * chArrayListCopy         - Creates an exact copy of a specified array list.
 * chArrayListDestroy      - Frees all memory resources associated with an array
 *                           list.
 * chArrayListClear        - Clears all elements from a specified array list.
 * chArrayListAddAt        - Inserts an element at a specified index, elements
 *                           will be shifted to make place.
 * chArrayListAddFirst     - Inserts an element at the beginning of the array
 *                           list, elements will be shifted to make place.
 * chArrayListAddLast      - Inserts an element at the end of the array list.
 * chArrayListRemoveAt     - Removes an element at the specified index, elements
 *                           elements will be shifted as a result.
 * chArrayListRemoveFirst  - Removes an element from the beginning of the array
 *                           list, elements will be shifted as a result.
 * chArrayListRemoveLast   - Removes an element from the end of the array list
 * chArrayListGetAt        - Accesses the element at the specified index.
 * chArrayListGetFirst     - Accesses the first element of the array list.
 * chArrayListGetLast      - Accesses the last element of the array list.
 * chArrayListIsFull       - Returns if the array list reached its max capacity.
 * chArrayListIsEmpty      - Returns true if the array list contains no elements.
 */
typedef struct ch_array_list_t {
	int* elements;   //elements[0] - the origin of the oldest movement
					//elements[1] - the target elements[2] - the eaten piece and so on
	int actualSize;
	int maxSize;  //the maximum number of movements recorded is maxsize/3
} CHArrayList;

/**
 * A type used for errors
 */
typedef enum ch_array_list_message_t {
	CH_ARRAY_LIST_SUCCESS,
	CH_ARRAY_LIST_INVALID_ARGUMENT,
	CH_ARRAY_LIST_FULL,
	CH_ARRAY_LIST_EMPTY
} CH_ARRAY_LIST_MESSAGE;

/**
 *  Creates an empty array list with the specified maximum capacity of moves.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
CHArrayList* chArrayListCreate(int maxSize);

/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
CHArrayList* chArrayListCopy(CHArrayList* src);

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void chArrayListDestroy(CHArrayList* src);

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * CH_ARRAY_LIST_SUCCESS otherwise
 */
CH_ARRAY_LIST_MESSAGE chArrayListClear(CHArrayList* src);

/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem_org  - the the origin value of the new element to be inserted
 * @param elem_org  - the the target value of the new element to be inserted
 * @param elem_org  - the the piece value of the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * CH_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * CH_ARRAY_LIST_SUCCESS - otherwise
 */
CH_ARRAY_LIST_MESSAGE chArrayListAddAt(CHArrayList* src, int elem_org,int elem_tar, int elem_piece, int index);

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem_org  - the the origin value of the new element to be inserted
 * @param elem_org  - the the target value of the new element to be inserted
 * @param elem_org  - the the piece value of the new element to be inserted
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * CH_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * CH_ARRAY_LIST_SUCCESS - otherwise
 */
 CH_ARRAY_LIST_MESSAGE chArrayListAddFirst(CHArrayList* src,int elem_org,int elem_tar, int elem_piece);

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem_org  - the the origin value of the new element to be inserted
 * @param elem_org  - the the target value of the new element to be inserted
 * @param elem_org  - the the piece value of the new element to be inserted
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * CH_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * CH_ARRAY_LIST_SUCCESS - otherwise
 */
CH_ARRAY_LIST_MESSAGE chArrayListAddLast(CHArrayList* src, int elem_org,int elem_tar, int elem_piece);

/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param index - The index from where the element will be removed.
 *                The index is 0-based.
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * CH_ARRAY_LIST_EMPTY - if the source array list is empty
 * CH_ARRAY_LIST_SUCCESS - otherwise
 */
CH_ARRAY_LIST_MESSAGE chArrayListRemoveAt(CHArrayList* src, int index);

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * CH_ARRAY_LIST_EMPTY - if the source array list is empty
 * CH_ARRAY_LIST_SUCCESS - otherwise
 */
CH_ARRAY_LIST_MESSAGE chArrayListRemoveFirst(CHArrayList* src);

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * CH_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * CH_ARRAY_LIST_EMPTY - if the source array list is empty
 * CH_ARRAY_LIST_SUCCESS - otherwise.
 */
CH_ARRAY_LIST_MESSAGE chArrayListRemoveLast(CHArrayList* src);

/**
 * Returns the element at the specified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, a pointer to the int[3] element
 */
int* chArrayListGetAt(CHArrayList* src, int index);

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, a pointer to the int[3] element at the beginning of the list is returned.
 */
int* chArrayListGetFirst(CHArrayList* src);

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise,  a pointer to the int[3] element at the end of the list is returned.
 */
int* chArrayListGetLast(CHArrayList* src);


/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool chArrayListIsFull(CHArrayList* src);

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool chArrayListIsEmpty(CHArrayList* src);



#endif
