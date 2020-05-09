#ifndef Dlist_OL70 /*header guard*/
#define Dlist_OL70
/**************************************************************
* The functions in this file handle a Doubly Linked List data *
* structure. It will be denoted as Dlist.                    *
*          													  *
*      														  *
* Written by: OL-70   										  *
* Last update: 10:40 , 17.07.2019                			  *
***************************************************************/

#include <stddef.h>

typedef struct dlist_node_s *dlist_iter_t;

typedef struct dlist_s dlist_t;

typedef int (*cmd_func_t)(void *data, void *arg);
typedef int (*comp_func_t)(const void *data1, const void *data2);

/*******************************************************************************
* Allocates the memory for dlist
* dlistDestroy must be called after the work with the
* dlist is done.
*
* Return Value: A pointer to the New DList.
*******************************************************************************/
dlist_t *DListCreate();

/*******************************************************************************
* Releases the memory that has been allocted for the DList
*
* dllist - a pointer to the Doubly Link List (Not NULL).
*******************************************************************************/
void DListDestroy(dlist_t *dlist);

/*******************************************************************************
* dllist - a pointer to the Doubly Link List (Not NULL).
*
*Return Value: the number of members in the DList.
*******************************************************************************/
size_t DListSize(const dlist_t *dlist);

/*******************************************************************************
 * DListIsEmpty - checks if the list is empty.  O(1)
 *
 * dllist - a pointer to the Doubly Link List (Not NULL).
 *
 * Return - TRUE if the DList is empty and FALSE otherwise.
 ******************************************************************************/
int DListIsEmpty(const dlist_t *dlist);

/*******************************************************************************
* DListPushFront - function receives a list and inserts a new member
* which contains data to the front location.
*
* dllist - a pointer to the Doubly Link List (Not NULL).
*
* data - the data the new member will hold.
*
* Returns - iterator - pointing to the position of the new member.
* If failed, NULL will be returned.
*******************************************************************************/
dlist_iter_t DListPushFront(dlist_t *dlist, void *data);

/*******************************************************************************
* DListPushBack - function receives a list and inserts a new member
* which contains data to the last location.
*
* dllist - a pointer to the Doubly Link List (Not NULL).
*
* data - the data the new member will hold.
*
* Returns - iterator - pointing to the position of the new member.
* If failed, NULL will be returned.
*******************************************************************************/
dlist_iter_t DListPushBack(dlist_t *dlist, void *data);

/*******************************************************************************
* DListPopFront - function receives a list, and removes
* the member in the front position in that list.
* dlist - a pointer to the Doubly Link List (Not NULL).
*
* Returns the data contained in the removed element.
* If fails - NULL is returned.
*******************************************************************************/
void *DListPopFront(dlist_t *dlist);

/*******************************************************************************
* DListPopBack - function receives a list, and removes
* the member in the last position in that list.
* dllist - a pointer to the Doubly Link List (Not NULL).
*
* Returns the data contained in the removed element.
* If fails - NULL is returned.
*******************************************************************************/
void *DListPopBack(dlist_t *dlist);

/*******************************************************************************
* Executes a command (cmd) given by the user with arg in given range.
* cmd - command function given by the user
*
* from - iterator that specifies from where to do the command (Not NULL).
* to - iterator that specifies until where to do the command (Not NULL).
* make sure from comes before to in the List
*
*******************************************************************************/
int DListForEach(dlist_iter_t begin, dlist_iter_t end, cmd_func_t cmd,
                                                        void *param);
/*******************************************************************************
* Executes comparison (comp) given by the user with arg in given range.
* comp - comparison function given by the user
*
* from - iterator that specifies from where to do the comparison (Not NULL).
* to - iterator that specifies until where to do the comparison (Not NULL).
* make sure from comes before to in the List
*
* Return value: iterator to the found data. if no match is found - the tail of
* the list is returned.
*******************************************************************************/
dlist_iter_t DListFind(dlist_iter_t begin, dlist_iter_t end, comp_func_t comp,
                                                            const void *param);

/*******************************************************************************
* Executes splice of a given portion of list into a given place in the
* destination list, denoted by where.
*
* from - iterator that specifies from where to do the command (Not NULL).
* to - iterator that specifies until where to do the command (Not NULL).
* make sure from comes before to in the List
* where - iterator that specifies where to do the splice (Not NULL).
*
* Return value: iterator to last copied data.
*******************************************************************************/
dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where);

/*******************************************************************************
* iter - iterator that specifies from where to get the data (Not NULL).
*
* Return value: the data in the given iterator.
*******************************************************************************/
void *DListGetData(const dlist_iter_t iter);

/*******************************************************************************
* dllist - a pointer to the Doubly Link List (Not NULL).
*
* Return value: iterator to the beginning of a given list.
*******************************************************************************/
dlist_iter_t DListBegin(const dlist_t *dlist);

/*******************************************************************************
* dllist - a pointer to the Doubly Link List (Not NULL).
*
* Return value: iterator to the end of a given list.
*******************************************************************************/
dlist_iter_t DListEnd(const dlist_t *dlist);

/*******************************************************************************
* Inserted new member to the list before the given iterator.
*
* dllist - a pointer to the Doubly Link List (Not NULL).
* iter - iterator that specifies before which member to insert the new member
* (Not NULL).
* data - pointer to the data to be save in the new member in the list.
* (Not NULL).
*
* Return value: iterator to the inserted member of the list. If the operation
* is failed - the tail of the list is returned.
*******************************************************************************/
dlist_iter_t DListInsert(dlist_t *dlist, dlist_iter_t iter, void *data);

/*******************************************************************************
* Erase member from the list in a given iterator.
*
* iter - iterator that specifies after which member to erase (Not NULL).
*
* Return value: iterator to the member atfer the member to erase. If the
* operation is failed - the tail of the list is returned.
*******************************************************************************/
dlist_iter_t DListRemove(dlist_iter_t iter);

/*******************************************************************************
* iter - iterator that specifies after which member to erase (Not NULL).
*
* Return value: iterator to next member of the list.
*******************************************************************************/
dlist_iter_t DListNext(dlist_iter_t iter);

/*******************************************************************************
* iter - iterator that specifies after which member to erase (Not NULL).
*
* Return value: iterator to previous member of the list.
*******************************************************************************/
dlist_iter_t DListPrev(dlist_iter_t iter);

/*******************************************************************************
* Checks if iter1 is the same as iter2
*
* iter1, iter2 - iterator that pointing on the position of the given
* members (Not NULL).
*
* Return value: TRUE (1) if the same, FALSE (0) otherwise.
*******************************************************************************/
int DListIsSameIter(const dlist_iter_t iter1, const dlist_iter_t iter2);

#endif  /*DList_OL70*/
