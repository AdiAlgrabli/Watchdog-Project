#ifndef SortedList_OL70 /*header guard*/
#define SortedList_OL70
/**************************************************************
* The functions in this file handle a Sorted List data        *
* structure. It will be denoted as SortedList.                *
*          													  *
*      														  *
* Written by: OL-70   										  *
* Last update: 08:45 , 15.07.2019                			  *
***************************************************************/

#include <stddef.h>
#include "dlist.h"  /* dlist_itr_t */

typedef struct sorted_list_s sorted_list_t;

typedef struct  sorted_list_iter_s
{
    dlist_iter_t iter;
} sorted_list_iter_t;

typedef int (*sorted_cmd_func_t)(void* data, void *arg);
typedef int (*is_before_func_t)(const void *data1, const void *data2,
                                                            const void *param);


/*******************************************************************************
* Allocates memory for SortedList.
* SortedListDestroy must be called after the work with the SortedList is done
* comp - compare function that the user can add
*
* Return Value: A pointer to the new SortedList.
*******************************************************************************/
sorted_list_t *SortedListCreate(is_before_func_t is_before, void *param);

/*******************************************************************************
* Releases the memory that has been allocted for the SortedList
*
* SortedLlist - a pointer to the sorted list (Not NULL).
*******************************************************************************/
void SortedListDestroy(sorted_list_t *sorted_list);

/*******************************************************************************
* Counts the elements in the list.
* sorted_list - a pointer to the sorter list (Not NULL).
*
* Return Value: the number of members in the SortedList.
*******************************************************************************/
size_t SortedListSize(const sorted_list_t *sorted_list);

/*******************************************************************************
 * SortedListIsEmpty - checks if the list is empty.  O(1)
 *
 * sorted_list - a pointer to the sorter list (Not NULL).
 *
 * Return - TRUE if the SortedList is empty and FALSE otherwise.
 ******************************************************************************/
int SortedListIsEmpty(const sorted_list_t *sorted_list);

/*******************************************************************************
* sorted_list - a pointer to the sorter list (Not NULL).
*
Return value: iterator to the end of a given list. If the list is empty
* - bad iterator is returned.
*******************************************************************************/
sorted_list_iter_t SortedListBegin(sorted_list_t *sorted_list);

/*******************************************************************************
* sorted_list - a pointer to the sorter list (Not NULL).
*
* Return value: iterator to the end of a given list. If the list is empty
* - bad iterator is returned.
*******************************************************************************/
sorted_list_iter_t SortedListEnd(sorted_list_t *sorted_list);

/*******************************************************************************
* iter - iterator that specifies after which member to erase (Not NULL).
*
* Return value: iterator to next member of the list.
*******************************************************************************/
sorted_list_iter_t SortedListNext(sorted_list_iter_t iter);

/*******************************************************************************
* iter - iterator that specifies after which member to erase (Not NULL).
*
* Return value: iterator to previous member of the list.
*******************************************************************************/
sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter);

/*******************************************************************************
* Checks if iter1 is the same as iter2
*
* iter1, iter2 - iterator that pointing on the position of the given
* members (Not NULL).
*
* Return value: TRUE (1) if the same, FALSE (0) otherwise.
*******************************************************************************/
int SortedListIsSame(const sorted_list_iter_t iter1,
                                                const sorted_list_iter_t iter2);

/*******************************************************************************
* iter - iterator that specifies from where to get the data (Not NULL).
*
* Return value: the data in the given iterator.
*******************************************************************************/
void *SortedListGetData(sorted_list_iter_t iter);

/*******************************************************************************
* Inserts new member to the list before the given iterator.
*
* sorted_list - a pointer to the sorter list (Not NULL).
* iter - iterator that specifies before which member to insert the new member
* (Not NULL).
* data - pointer to the data to be saved in the new member in the list.
* (Not NULL).
*
* Return value: iterator to the inserted member in the list. If the operation
* is failed - bad iterator is returned.
*******************************************************************************/
sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data);

/*******************************************************************************
* Erase member from the list in a given iterator.
*
* iter - iterator that specifies after which member to erase (Not NULL).
*
* Return value: iterator to the member atfer the member to erase. If the
* operation is failed - bad iterator is returned.
*******************************************************************************/
sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter);

/*******************************************************************************
* Removes the member that is in the first position in the list.
*
* sorted_list - a pointer to the sorted list (Not NULL).
* Return value: a void pointer to the removed data
*******************************************************************************/
void *SortedListPopFront(sorted_list_t *sorted_list);

/*******************************************************************************
* Removes the member that is in the last position in the list.
*
* sorted_list - a pointer to the sorted list (Not NULL).
* Return value: a void pointer to the removed data
*******************************************************************************/
void *SortedListPopBack(sorted_list_t *sorted_list);

/*******************************************************************************
* Executes a command (cmd) given by the user with arg in given range.
* cmd - command function given by the user
*
* from - iterator that specifies from where to do the command (Not NULL).
* to - iterator that specifies until where to do the command (Not NULL).
* WARNING - make sure from comes before to in the List
*
* Return value: STOP, CONTINUE.
*******************************************************************************/
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
                      sorted_cmd_func_t cmd, void *arg);

/*******************************************************************************
* Executes merge of a given range of list into a given place in the
* destination list, denoted by dest.
*
* dest - iterator that specifies which list to merge to.
* from - iterator that specifies from where to merge in the source list.
* to - iterator that specifies until where to merge in the source list.
* WARNING - make sure from comes before to in the List
*
* Return value: iterator to the found data. If no match was found - to is
* returned
*******************************************************************************/
void SortedListMerge(sorted_list_t *dest, sorted_list_t *src);

/*******************************************************************************
* Executes comparison (comp) given by the user with arg in given range.
* comp - comparison function given by the user
*
* from - iterator that specifies from where to do the comparison (Not NULL).
* to - iterator that specifies until where to do the comparison (Not NULL).
* make sure from comes before to in the List
*
* Return value: iterator to the found data. If no match was found - the tail of
* the list is returned.
*******************************************************************************/
sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list, sorted_list_iter_t from,
    sorted_list_iter_t to, const void *arg);

/*******************************************************************************
* Executes is_match given by the user with arg in given range.
* is_match should return 1 in case of a match.
*
* from - iterator that specifies from where to do the comparison (Not NULL).
* to - iterator that specifies until where to do the comparison (Not NULL).
* make sure from comes before to in the List
*
* Return value: iterator to the found data. If no match was found - 'to' is
* returned.
*******************************************************************************/
sorted_list_iter_t SortedListFindIf(
    sorted_list_iter_t from,
    sorted_list_iter_t to,
    int (*is_match)(const void *data, const void *param),
    const void *arg);

#endif  /*SortedList_OL70*/
