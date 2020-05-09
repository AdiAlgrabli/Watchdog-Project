#ifndef PQUEUE_OL70 /*header guard*/
#define PQUEUE_OL70
/**************************************************************
* The functions in this file handle a Priority Queue data      *
* structure, and will be denoted as PQ.                       *
*                    										  *
* Written by: OL-70   										  *
* Last update: 12:40 18.07.2019                    			  *
***************************************************************/
#include <stddef.h>

typedef struct pqueue_s pqueue_t;
typedef int (*pq_comp_func_t)(const void *data1, const void *data2, void *param);

/***************************************************************
* Allocates memory for PQ                                  *
* PQDestroy must be called after the work with the             *
* PQ is done.									               *
*														       *
* Return Value: A pointer to the New PQ                        *
****************************************************************/
pqueue_t *PQCreate(pq_comp_func_t comp, void *param);


/***************************************************************
* Releases the memory that has been allocated to the PQ        *
****************************************************************/
void PQDestroy(pqueue_t *pqueue);


/***************************************************************
* Returns a pointer to the element on top of the PQ            *
****************************************************************/
void *PQPeek(pqueue_t *pqueue);

/***************************************************************
* Releases the first element from the start of the PQ          *
*                                                              *
* Retrun Value: pointer to the released element                *
****************************************************************/
void *PQDequeue(pqueue_t *pqueue);


/**************************************************************
* Pushes data to the PQ.              			    	      *
* data is a pointer to an element                  	          *
*														      *
* Return Value: 1 if succeeded 0 if failed	       		      *
***************************************************************/
int PQEnqueue(pqueue_t *pqueue, void *data);

/**************************************************************
* Returns the number of elements in the PQ.       		      *
***************************************************************/
size_t PQSize(const pqueue_t *pqueue);

/**************************************************************
* Return TRUE (1) if the PQ is empty.			  	     	  *
* Return FALSE (0) otherwise.                		  		  *
***************************************************************/
int PQIsEmpty(const pqueue_t *pqueue);

/**************************************************************
* Clears the PQ, by emptying it from elements.      		  *
* NOTE - The PQ is not destroyed. It can be filled with New   *
* elements.                                                   *
***************************************************************/
void PQClear(pqueue_t *pqueue);

/**************************************************************
* Erases an element in the PQ according to a certain parameter*
*                                                             *
* is_match - a function that checks whether an element meets  *
* the criteria to be erased. Subsequently, that element will  *
* be erased.                                                  *
***************************************************************/
void *PQErase(pqueue_t *pqueue,
    int (*is_match)(const void *data, const void *param), void *param);

#endif /*PQUEUE_OL70*/
