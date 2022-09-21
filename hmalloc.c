#include "hmalloc.h"
#include <stdlib.h>
/*You may include any other relevant headers here.*/
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/*Add additional data structures and globals here as needed.*/
void *free_list = NULL;

/* traverse
 * Start at the free list head, visit and print the length of each
 * area in the free pool. Each entry should be printed on a new line.
 */
void traverse(){
   /* Printing format:
	 * "Index: %d, Address: %08x, Length: %d\n"
	 *    -Index is the position in the free list for the current entry. 
	 *     0 for the head and so on
	 *    -Address is the pointer to the beginning of the area.
	 *    -Length is the length in bytes of the free area.
	 */
	uintptr_t *list;
	int i;
	i = 0;
	list = (uintptr_t*)free_list;
	while(list){
		printf("Index: %d, Address: %08x, Length: %ld\n", i, (int)list, (int)list[0]);
		i++;
		list = (uintptr_t*)(list[1]);
	}
}

/* hmalloc
 * Allocation implementation.
 *    -will not allocate an initial pool from the system and will not 
 *     maintain a bin structure.
 *    -permitted to extend the program break by as many as user 
 *     requested bytes (plus length information).
 *    -keeps a single free list which will be a linked list of 
 *     previously allocated and freed memory areas.
 *    -traverses the free list to see if there is a previously freed
 *     memory area that is at least as big as bytes_to_allocate. If
 *     there is one, it is removed from the free list and returned 
 *     to the user.
 */
void *hmalloc(int bytes_to_allocate){
	uintptr_t *create, *list, *lookAhead, *lookBehind;
	list = (uintptr_t*)free_list;
	lookBehind = list;

	while(list){ //while list is not empty
		if(list[0] > bytes_to_allocate){ //if the current location is big enough
			lookAhead = (uintptr_t*)list[1]; //the location of the next field
			if(lookBehind == list){ //if first in free_list
				free_list = (uintptr_t*)list[1]; //Either null or next first in list
				list[1] = 0;
				return list + 2; //return start of memory segment
			}
			list[1] = 0; //nulling current next address
			lookBehind[1] = (uintptr_t)lookAhead; //setting next address of previous to ahead 
			return list + 2; //returning the chosen memory segment
		}
		else{ //else keep looping
			lookBehind = list;
			list = (uintptr_t*)list[1];
		}

		if((!list[1])){//Terminate if last in list
			break;
		}
	}

	create = (uintptr_t*)sbrk(bytes_to_allocate + 2);
	create[0] = bytes_to_allocate + 2;
	create[1] = 0;
	return create + 2;
}

/* hcalloc
 * Performs the same function as hmalloc but also clears the allocated 
 * area by setting all bytes to 0.
 */
void *hcalloc(int bytes_to_allocate){
	void *newBrk;
	unsigned char *agh;
	newBrk = hmalloc(bytes_to_allocate);
	agh = (unsigned char*)(newBrk);
	for(int i = 0; i < bytes_to_allocate; i++){
		agh[i] = 0;
	}
   return newBrk;
}

/* hfree
 * Responsible for returning the area (pointed to by ptr) to the free
 * pool.
 *    -simply appends the returned area to the beginning of the single
 *     free list.
 */
void hfree(void *ptr){
	uintptr_t *intPtr;
	uintptr_t *intList;
	intList = free_list;
	intPtr = ((uintptr_t *)ptr) - 2; // shift start of pointer to start of internal segment
	if (!intList) {
		intPtr[1] = 0; // if list empty, make end of list
	}
	else {
		intPtr[1] = (uintptr_t)intList; // add current free_list head to address slot in mem seg <====== it was this line
	}
	free_list = intPtr; // change list to start with new segment
}

/* hrealloc
 * Calls hmalloc to make a new segment
 * Copies old segment over, and frees it
 *
*/
void *hrealloc(int bytes_to_allocate, void *ptr){
	uintptr_t *create;
	unsigned char *copyData;
	unsigned char *copiedData;
	copiedData = (unsigned char*)ptr;
	create = (uintptr_t*)sbrk(bytes_to_allocate + 2);
	create[0] = bytes_to_allocate + 2;
	create[1] = 0;
	copyData = (unsigned char*)(create + 2); //start copy at memory segment

	for(int i = 0; i < bytes_to_allocate; i++){
		copyData[i] = copiedData[i];
	}

	hfree(ptr);
	return create + 2;
}

/*You may add additional functions as needed.*/
