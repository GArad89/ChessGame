#include "CHArrayList.h"
#include <stdlib.h>
#include <stdbool.h>

CHArrayList* chArrayListCreate(int maxSize){
	CHArrayList* arr_list	=	(CHArrayList*) malloc(sizeof(CHArrayList));
	if(arr_list	==	NULL){ //memory allocation error
		printf("Error: memory allocation error occured in chArrayListCreate\n");
		return NULL;
	}
	arr_list->actualSize	=	0;
	arr_list->maxSize	=	3*maxSize;  //the maxSize input is the maximum number of moves kept (of both players).
	//arr_list's max size is three times the size, because it keeps the previous position of the piece and it's new one and the eaten piece if any.
	arr_list->elements = (int*)malloc(3*maxSize*sizeof(int));
	if(arr_list->elements == NULL){  //memory allocation error
		printf("Error: memory allocation error occured in chArrayListCreate\n");
		free(arr_list);
		return NULL;
	}
	return arr_list;
}

CHArrayList* chArrayListCopy(CHArrayList* src){
	if(src == NULL){
		printf("Error: null pointer argument recieved by chArrayListCopy\n");
	}
	CHArrayList* arr_list	=	(CHArrayList*) malloc(sizeof(CHArrayList));

	if((arr_list	==	NULL)){ //memory allocation error
		printf("Error: memory allocation error occured in chArrayListCopy\n");
		return NULL;
	}

	arr_list->actualSize	=	src->actualSize;
	arr_list->maxSize	=	src->maxSize;
	arr_list->elements = (int*)malloc(src->maxSize*sizeof(int));

	if(arr_list->elements	==	0){
		return 0;
	}
	int i;
	for (i	=	0; i	<	src->actualSize; i++){
		arr_list->elements[i]=src->elements[i];
	}

	return arr_list;
}

void chArrayListDestroy(CHArrayList* src){
	if(src	==	NULL){
		return;
	}
	if(src->elements != NULL){
		free(src->elements);
	}
	free(src);
}

CH_ARRAY_LIST_MESSAGE chArrayListClear(CHArrayList* src){
	if(src	==	NULL){
		return CH_ARRAY_LIST_INVALID_ARGUMENT;
	}

	src->actualSize	=	0;
	return CH_ARRAY_LIST_SUCCESS;
}

CH_ARRAY_LIST_MESSAGE chArrayListAddAt(CHArrayList* src, int elem_org,int elem_tar, int elem_piece, int index){

	if((src	==	NULL)||(3*index	>=	src->maxSize)){
		return CH_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if(src->actualSize	==	src->maxSize){
		return CH_ARRAY_LIST_FULL;
	}

	int temp_org	=	src->elements[3*index];
	int temp_tar	=	src->elements[3*index+1];
	int temp_piece	=	src->elements[3*index+2];
	int i;
	src->elements[3*index]	=	elem_org;
	src->elements[3*index+1]	=	elem_tar;
	src->elements[3*index+2]	=	elem_piece;
	for (i	=	3*index+3; i	<=	src->actualSize; i+=3){
		elem_org	=	src->elements[i];
		elem_tar	=	src->elements[i+1];
		elem_piece	=	src->elements[i+2];
		src->elements[3*i]	=	temp_org;
		src->elements[3*i+1]	=	temp_tar;
		src->elements[3*i+2]	=	temp_piece;
		temp_org	=	elem_org;
		temp_tar	=	elem_tar;
		temp_piece	=	elem_piece;
	}

	src->actualSize+=3;
	return CH_ARRAY_LIST_SUCCESS;
}

CH_ARRAY_LIST_MESSAGE chArrayListAddFirst(CHArrayList* src, int elem_org,int elem_tar,int elem_piece){
	return chArrayListAddAt(src,elem_org,elem_tar,elem_piece,0);
}

CH_ARRAY_LIST_MESSAGE chArrayListAddLast(CHArrayList* src, int elem_org,int elem_tar,int elem_piece){
	if((src	==	NULL)){
		return CH_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if(src->actualSize	==	src->maxSize){
		return CH_ARRAY_LIST_FULL;
	}

	src->elements[src->actualSize]	=	elem_org;
	src->elements[src->actualSize+1]	=	elem_tar;
	src->elements[src->actualSize+2]	=	elem_piece;
	src->actualSize+=3;
	return CH_ARRAY_LIST_SUCCESS;
}
CH_ARRAY_LIST_MESSAGE chArrayListRemoveAt(CHArrayList* src, int index){

	if((src	==	NULL)||(index	>=	(int)(src->maxSize/3))){
		return CH_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if(src->actualSize	==	0){
		return CH_ARRAY_LIST_EMPTY;
	}
	int i;
	for (i	=	3*(index+1); i	<	src->actualSize-2; i+=3){
		src->elements[i-3]	=	src->elements[i];
		src->elements[i-2]	=	src->elements[i+1];
		src->elements[i-1]	=	src->elements[i+2];
	}

	src->actualSize-=3;
	return CH_ARRAY_LIST_SUCCESS;
}

CH_ARRAY_LIST_MESSAGE chArrayListRemoveFirst(CHArrayList* src){
	return chArrayListRemoveAt(src,0);
}

CH_ARRAY_LIST_MESSAGE chArrayListRemoveLast(CHArrayList* src){
	if((src	==	NULL)){
		return CH_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if(src->actualSize	==	0){
		return CH_ARRAY_LIST_EMPTY;
	}

	src->actualSize-=3;
	return CH_ARRAY_LIST_SUCCESS;
}

int* chArrayListGetAt(CHArrayList* src, int index){
	return &src->elements[3*index];
}

int* chArrayListGetFirst(CHArrayList* src){
	return &(src->elements[0]);
}

int* chArrayListGetLast(CHArrayList* src){
	return &(src->elements[src->actualSize-3]);
}


bool chArrayListIsFull(CHArrayList* src){
	if((src	==	NULL)){
		return false;
	}
	if((src->actualSize	<	src->maxSize)){
		return false;
	}

	return true;
}
bool chArrayListIsEmpty(CHArrayList* src){
	if((src	==	NULL)||(src->actualSize	>	0)){
		return false;
	}

	return true;
}

