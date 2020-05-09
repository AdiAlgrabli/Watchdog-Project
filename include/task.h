#ifndef Task_OL70 /*header guard*/
#define Task_OL70
/*******************************************************************************
* The functions in this file handle a task
* structure. It will be denoted as SortedList.
*
*
* Written by: OL-70
* Last update: 14:00 , 22.07.2019
*******************************************************************************/

#include <stddef.h>
#include <time.h>

#include "UID.h"

typedef enum {STOP, REPEAT} task_run_t;

typedef int (*is_match)(const void *data, const void *param);
typedef task_run_t (*action_func_t)(void *param);

typedef struct task_s task_t;

/*******************************************************************************
* Allocate memory for task.
*
* func - function to be executed by the task.
* interval - num of seconds between two runs of the task.
* param - parameter of the task.
*
* Returns a pointer to the task. If allocation has been faild - NULL is return.
*******************************************************************************/
task_t *TaskCreate(size_t interval, action_func_t func, void *param);

/*******************************************************************************
* Realeses the memory allocated to the task.
*
* task - pointer to task.
*******************************************************************************/
void TaskDestroy(task_t *task);

/*******************************************************************************
 * Execute a given task.
 *
 * task - pointer to task (not NULL).
 *
 * Return - STOP / REPEAT, according to the action func return value.
 ******************************************************************************/
task_run_t TaskRun(task_t *task);

/*******************************************************************************
* Execute a compersion between the priority of two given tasks.
*
* task1, task2 - pointers to tasks (not NULL).
*
* Return value: TRUE(1) if task1 is before task 2. 0 otherwise.
*******************************************************************************/
int TaskIsBefore(const task_t *task1, const task_t *task2, void *param);

/*******************************************************************************
* Update time to run of a task.
*
* task - pointer to task (not NULL).
*******************************************************************************/
void TaskUpdateTimeToRun(task_t *task);

/*******************************************************************************
* task - a pointer to task (Not NULL).
*
* Return value: time to run of a task.
*******************************************************************************/
time_t TaskGetTimeToRun(const task_t *task);

/*******************************************************************************
* task - a pointer to task (Not NULL).
*
* Return value: UID of the task.
*******************************************************************************/
UID_t TaskGetUID(const task_t *task);

/*******************************************************************************
* task - a pointer to task (Not NULL).
* uid - Universal Identifier.
*
* checks if the given uid is match to the give task.
*
* Return value: TRUE(1) if match, FALSE(0) otherwise.
*******************************************************************************/
int TaskIsMatch(const task_t *task, UID_t uid);

#endif  /*Task_OL70*/
