#ifndef Scheduler_OL70 /*header guard*/
#define Scheduler_OL70
/*******************************************************************************
* The functions in this file handle a Scheduler
* structure. It will be denoted as Scheduler.
*
*
* Written by: OL-70
* Last update: 14:00 , 22.07.2019
*******************************************************************************/

#include <stddef.h>

#include "UID.h"

typedef enum {ERROR = -1, STOPPED = 0, SUCCESS = 1} sched_run_t;

typedef struct sched_s sched_t;
typedef int (*op_func_t)(void *param);


/*******************************************************************************
* Allocate memory for scheduler.
*
* Returns a pointer to the scheduler. If allocation has been faild - NULL is
* return.
*******************************************************************************/
sched_t *SchedCreate();

/*******************************************************************************
* Realeses the memory allocated to the scheduler.
*
* sched - pointer to scheduler.
*******************************************************************************/
void SchedDestroy(sched_t *sched);

/*******************************************************************************
 * Execute all the tasks in a given scheduler.
 *
 * sched - pointer to scheduler (not NULL).
 *
 * Return value: ERROR \ STOPPED \ SUCCESS
 ******************************************************************************/
sched_run_t SchedRun(sched_t *sched);

/*******************************************************************************
* Execute a compersion between the priority of two given tasks.
*
* sched - pointer to scheduler (not NULL).
*
* Return value: num of tasks in a given scheduler.
*******************************************************************************/
size_t SchedSize(sched_t *sched);

/*******************************************************************************
* Clears the scheduler, by emptying it from tasks.
* NOTE - The scheduler is not destroyed. It can be filled with new tasks.
*
* sched - pointer to scheduler (not NULL).
*******************************************************************************/
void SchedClear(sched_t *sched);

/*******************************************************************************
* sched - pointer to scheduler (not NULL).
*
* Return value: TRUE(1) if the sched is empty, FALSE(0) otherwise.
*******************************************************************************/
int SchedIsEmpty(sched_t *sched);

/*******************************************************************************
* sched - pointer to scheduler (not NULL).
* func -  operation function of the task.
* param - parameter of the task.
* interval - time interval of the task.
*
* Return value: UID of the task.
*******************************************************************************/
UID_t SchedAddTask(sched_t *sched, op_func_t func, void *param, size_t interval);

/*******************************************************************************
* Remove task from the scheduler.
*
* uid - UID the task.
* sched - pointer to scheduler (not NULL).
*******************************************************************************/
void SchedRemoveTask(sched_t *sched, UID_t uid);

/*******************************************************************************
* This function cause the scheduler to stop.
*
* sched - pointer to scheduler (not NULL).
*******************************************************************************/
void SchedStop(sched_t *sched);

#endif  /*Scheduler_OL70*/
