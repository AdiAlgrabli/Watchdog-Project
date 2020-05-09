#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "task.h"
#include "pqueue.h"
#include "sched.h"

#define UNUSED(expr) (void)(expr)

static int sched_exist = 0;

struct sched_s
{
    pqueue_t *pqueue;
    task_t *curr_task;
    int stop_flag;
};

static int CompTime(const void *data1, const void *data2, void *param);
static int IsMatchUID(const void *data, const void *param);

sched_t *SchedCreate()
{
    if (!sched_exist)
    {
        sched_t *sched = malloc(sizeof(sched_t));
        if(NULL == sched)
        {
            return (NULL);
        }

        sched_exist = 1;

        sched->pqueue = PQCreate(CompTime, NULL);
        if (NULL == sched->pqueue)
        {
            free(sched);
            sched = NULL;

            return (NULL);
        }

        sched->curr_task = NULL;
        sched->stop_flag = 0;

        return (sched);
    }

    return (NULL);
}

void SchedDestroy(sched_t *sched)
{
    assert(sched);

    SchedClear(sched);
    PQDestroy(sched->pqueue);
    sched_exist = 0;
    free(sched);
}

sched_run_t SchedRun(sched_t *sched)
{
    task_run_t status = REPEAT;
    int enqueue_status = 0;
    int need_to_sleep = 0;

    assert(sched);

    sched->stop_flag = 0;

    while (!SchedIsEmpty(sched) && !sched->stop_flag)
    {
        sched->curr_task = PQDequeue(sched->pqueue);
        need_to_sleep = TaskGetTimeToRun(sched->curr_task) - time(NULL);

        if (need_to_sleep > 0)
        {
            sleep(need_to_sleep);
        }

        TaskUpdateTimeToRun(sched->curr_task);

        status = TaskRun(sched->curr_task);
        if (STOP == status)
        {
            TaskDestroy(sched->curr_task);
        }
        else
        {
            enqueue_status = PQEnqueue(sched->pqueue, sched->curr_task);
            if (0 == enqueue_status)
            {
                return (ERROR);
            }
        }
    }

    if (sched->stop_flag)
    {
        return (STOPPED);
    }

    return (SUCCESS);

}

size_t SchedSize(sched_t *sched)
{
    assert(sched);

    return (PQSize(sched->pqueue));
}

void SchedClear(sched_t *sched)
{
    assert(sched);

    while (!SchedIsEmpty(sched))
    {
        sched->curr_task = PQDequeue(sched->pqueue);
        TaskDestroy(sched->curr_task);
    }
}

int SchedIsEmpty(sched_t *sched)
{
    assert(sched);

    return (PQIsEmpty(sched->pqueue));
}

UID_t SchedAddTask(sched_t *sched, op_func_t func, void *param, size_t interval)
{
    task_t *task = NULL;

    assert(sched);
    assert(func);

    task = TaskCreate(interval, (action_func_t)func, param);
    if (NULL == task)
    {
        return (BAD_UID);
    }

    if (PQEnqueue(sched->pqueue, task))
    {
        return (TaskGetUID(task));
    }

    return (BAD_UID);
}

void SchedRemoveTask(sched_t *sched, UID_t uid)
{
    task_t *task_to_remove = NULL;

    assert(sched);

    task_to_remove = (task_t *)PQErase(sched->pqueue, IsMatchUID, (void *)&uid);

    if (task_to_remove)
    {
        TaskDestroy(task_to_remove);
    }
}

void SchedStop(sched_t *sched)
{
    sched->stop_flag = 1;
}


/*______________________________Static Funcitons______________________________*/

static int IsMatchUID(const void *data, const void *param)
{
    task_t *task = (task_t *)data;
    UID_t *uid = (UID_t *)param;

    return (UIDIsSame(TaskGetUID(task), *uid));
}

static int CompTime(const void *data1, const void *data2, void *param)
{
    assert(data1);
    assert(data2);

    UNUSED(param);

    return ((TaskGetTimeToRun((task_t *)data1) - TaskGetTimeToRun((task_t *)data2)));
}
