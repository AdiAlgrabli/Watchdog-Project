#ifndef UID_OL70 /*header guard*/
#define UID_OL70
/**************************************************************
* The functions in this file handle a Universal Identifier    *
* data structure. It will be denoted as Dlist.                *
*          													  *
*      														  *
* Written by: OL-70   										  *
* Last update: 13:30 , 16.07.2019                			  *
***************************************************************/
#include <stddef.h> /* size_t */
#include <sys/types.h> /* pid_t */

typedef struct UID_s UID_t;

struct UID_s
{
    time_t time_stamp;
    size_t counter;
    pid_t pid;
};

extern const UID_t BAD_UID;

/**************************************************************
* Initializes the necessary fields for the UID	              *
* Return Value: The UID.                                      *
***************************************************************/
UID_t UIDCreate();

/**************************************************************
 * Checks if one UID is identical to another one              *
   Return Value: 1 - if true, 0 - if false                    *
   ************************************************************/
int UIDIsSame(const UID_t uid1, const UID_t uid2);

/**************************************************************
 * Checks if one UID is an invalid UID                        *
   Return Value: 1 - if true, 0 - if false                    *
   ************************************************************/
int UIDIsBad(const UID_t uid);

#endif  /*UID_OL70*/
