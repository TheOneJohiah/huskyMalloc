#include "hmalloc.h"
/*You may include any other relevant headers here.*/

/*	main()
 *	Use this function to develop tests for hmalloc. You should not 
 *	implement any of the hmalloc functionality here. That should be
 *	done in hmalloc.c
 *	This file will not be graded. When grading I will replace main 
 *	with my own implementation for testing.*/
int main(int argc, char *argv[]){
	// some calls to hmalloc
	// ...
	// some calls to hfree
    int *p1 = hmalloc(4*sizeof(int));

    traverse();

    int *p2 = hmalloc(sizeof(int[4]));

    traverse();

    int *p3 = hmalloc(4*sizeof *p3);

    traverse();

    hfree(p1);
    traverse();
    hfree(p2);
    traverse();
    hfree(p3);
    traverse();


    int *p4 = hmalloc(4*sizeof *p4);

    traverse();
    hfree(p4);
    traverse();

    long int *p5 = hcalloc(4*sizeof *p5);

    traverse();
    hfree(p5);
    traverse();

    int *p6 = hmalloc(4*sizeof *p6);

    int *p7 = hrealloc(4*sizeof *p7, p6);
    traverse();
    hfree(p7);
    traverse();

}
