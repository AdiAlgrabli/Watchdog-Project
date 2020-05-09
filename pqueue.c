#include <assert.h>
#include <stdlib.h>

#include "sorted_list.h"
#include "pqueue.h"

#define UNUSED(expr) (void)(expr)

typedef struct comp_func_s
{
    pq_comp_func_t comp_func;
    void *param;
} comp_struct_t;


struct pqueue_s
{
    sorted_list_t *sorted_list;
    comp_struct_t *comp_struct;
};

static int IsBefore(const void *data1, const void *data2, const void *param);


pqueue_t *PQCreate(pq_comp_func_t cmp, void *param)
{
    pqueue_t *pqueue = NULL;
    comp_struct_t *comp_struct = NULL;

    assert(cmp);

    pqueue = malloc(sizeof(pqueue_t));
    if (NULL == pqueue)
    {
        return (NULL);
    }

    comp_struct = malloc(sizeof(comp_struct_t));
    if (NULL == comp_struct)
    {
        free(pqueue);

        return (NULL);
    }

    comp_struct->comp_func = cmp;
    comp_struct->param = param;
    pqueue->comp_struct = comp_struct;

    pqueue->sorted_list = SortedListCreate(IsBefore, comp_struct);

    if (NULL == pqueue->sorted_list)
    {
        free(pqueue->comp_struct);
        free(pqueue);

        return (NULL);
    }

    return (pqueue);
}

void PQDestroy(pqueue_t *pqueue)
{
    assert(pqueue);

    SortedListDestroy(pqueue->sorted_list);
    free(pqueue->comp_struct);
    free(pqueue);
}

void *PQPeek(pqueue_t *pqueue)
{
    assert(pqueue);

    if (PQIsEmpty(pqueue))
    {
        return (NULL);
    }

    return (SortedListGetData(SortedListBegin(pqueue->sorted_list)));
}

int PQEnqueue(pqueue_t *pqueue, void *data)
{
    sorted_list_iter_t iter = {NULL};

    assert(pqueue);

    iter = SortedListInsert(pqueue->sorted_list, data);

    return (!SortedListIsSame(iter, SortedListEnd(pqueue->sorted_list)));
}

void *PQDequeue(pqueue_t *pqueue)
{
    assert(pqueue);

    return (SortedListPopFront(pqueue->sorted_list));
}

size_t PQSize(const pqueue_t *pqueue)
{
    assert(pqueue);

    return (SortedListSize(pqueue->sorted_list));
}

int PQIsEmpty(const pqueue_t *pqueue)
{
    assert(pqueue);

    return (SortedListIsEmpty(pqueue->sorted_list));
}

void PQClear(pqueue_t *pqueue)
{
    assert(pqueue);

    while (!PQIsEmpty(pqueue))
    {
        PQDequeue(pqueue);
    }
}

void *PQErase(pqueue_t *pqueue,
             int (*is_match)(const void *data, const void *param), void *param)
{
    sorted_list_iter_t pq_iter = {NULL};
    void *iter_data = NULL;

    assert(pqueue);
    assert(is_match);

    pq_iter = SortedListFindIf(SortedListBegin(pqueue->sorted_list),
                              SortedListPrev(SortedListEnd(pqueue->sorted_list)),
                              is_match, param);

    iter_data = SortedListGetData(pq_iter);
    if (is_match(iter_data, param))
    {
        SortedListRemove(pq_iter);
    }

    return (iter_data);
}

static int IsBefore(const void *data1, const void *data2, const void *param)
{
    comp_struct_t *comp_func_struct = (comp_struct_t *)param;

    return (comp_func_struct->comp_func(data1, data2, comp_func_struct->param) < 0);
}
