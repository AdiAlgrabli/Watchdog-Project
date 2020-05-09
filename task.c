#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "task.h"

#define UNUSED(expr) (void)(expr)

struct task_s
{
    UID_t uid;
    size_t interval;
    time_t next_run;
    action_func_t func;
    void *param;
};

task_t *TaskCreate(size_t interval, action_func_t func, void *param)
{
    task_t *task = NULL;

    assert(func);

    task = malloc(sizeof(task_t));
    if (NULL == task)
    {
        return (NULL);
    }

    task->uid = UIDCreate();
    if (UIDIsBad(task->uid))
    {
        return (NULL);
    }

    task->interval = interval;
    task->next_run = time(NULL) + task->interval;
    task->func = func;
    task->param = param;

    return (task);
}

void TaskDestroy(task_t *task)
{
    assert(task);

    free(task);
}

task_run_t TaskRun(task_t *task)
{
    assert(task);

    return (task->func(task->param));
}

int TaskIsBefore(const task_t *task1, const task_t *task2, void *param)
{
    assert(task1);
    assert(task2);

    UNUSED(param);

    return (task1->next_run < task2->next_run);
}

void TaskUpdateTimeToRun(task_t *task)
{
    assert(task);

    task->next_run = time(NULL) + task->interval;
}

time_t TaskGetTimeToRun(const task_t *task)
{
    assert(task);

    return (task->next_run);
}

UID_t TaskGetUID(const task_t *task)
{
    assert(task);

    return (task->uid);
}

int TaskIsMatch(const task_t *task, UID_t uid)
{
    assert(task);

    return (UIDIsSame(task->uid, uid));
}
