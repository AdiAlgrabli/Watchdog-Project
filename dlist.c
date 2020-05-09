#include <stdlib.h>
#include <assert.h>
#include "dlist.h"

typedef struct dlist_node_s dll_node_t;

struct dlist_node_s
{
    dll_node_t *prev;
    dll_node_t *next;
    void *data;
};

struct dlist_s
{
    dll_node_t *head;
    dll_node_t *tail;
};

static dlist_iter_t DListInsertAfter(dlist_t *dlist, dlist_iter_t iter, void *data);

static dll_node_t *CreateNode(dll_node_t *prev, dll_node_t *next, void *data)
{
    dll_node_t *node = malloc(sizeof(dll_node_t));

    if (NULL == node)
    {
        return (NULL);
    }
    node->prev = prev;
    node->next = next;
    node->data = data;

    return (node);
}

dlist_t *DListCreate()
{
    dlist_t *dlist = malloc(sizeof(dlist_t));
    void *dummy_data = (void *)0xdeadbeef;

    if (NULL == dlist)
    {
        return (NULL);
    }

    dlist->head = CreateNode(NULL, NULL, dummy_data);
    if (NULL == dlist->head)
    {
        free(dlist);
        return (NULL);
    }

    dlist->tail = CreateNode(dlist->head, NULL, dummy_data);
    if (NULL == dlist->tail)
    {
        free(dlist->head);
        free(dlist);
        return (NULL);
    }

    dlist->head->next = dlist->tail;

    return (dlist);
}

void DListDestroy(dlist_t *dlist)
{
    if (!DListIsEmpty(dlist))
    {
        dll_node_t *node_to_remove = dlist->head->next;

        while (dlist->tail != node_to_remove)
        {
            node_to_remove = (dll_node_t *)DListRemove((dlist_iter_t)node_to_remove);
        }
    }
    free(dlist->head);
    free(dlist->tail);
    free(dlist);
}

size_t DListSize(const dlist_t *dlist)
{
    dll_node_t *runner = NULL;
    size_t counter = 0;

    assert(NULL != dlist);

    runner = dlist->head->next;
    while (runner != dlist->tail)
    {
        ++counter;
        runner = runner->next;
    }

    return (counter);
}

int DListIsEmpty(const dlist_t *dlist)
{
    assert(NULL != dlist);

    return (dlist->head->next == dlist->tail);
}

dlist_iter_t DListPushFront(dlist_t *dlist, void *data)
{
    assert(NULL != dlist);

    return ((dlist_iter_t)DListInsertAfter(dlist, (dlist_iter_t)dlist->head, data));
}

dlist_iter_t DListPushBack(dlist_t *dlist, void *data)
{
    assert(NULL != dlist);

    return ((dlist_iter_t)DListInsert(dlist, (dlist_iter_t)dlist->tail, data));
}

void* DListPopFront(dlist_t *dlist)
{
    assert(NULL != dlist);

    if (!DListIsEmpty(dlist))
    {
        void *data_to_return = dlist->head->next->data;

        DListRemove((dlist_iter_t)dlist->head->next);

        return (data_to_return);
    }

    return (NULL);
}

void* DListPopBack(dlist_t *dlist)
{
    assert(NULL != dlist);

    if (!DListIsEmpty(dlist))
    {
        void *data_to_return = dlist->tail->prev->data;

        DListRemove((dlist_iter_t)dlist->tail->prev);

        return (data_to_return);
    }

    return (NULL);
}

void *DListGetData(const dlist_iter_t iter)
{
    dll_node_t *iter_node = (dll_node_t *)iter;

    return (iter_node->data);
}

dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != where);
    assert(NULL != to->next);

    from->prev->next = to->next;
    to->next->prev = from->prev;
    from->prev = where->prev;
    to->next = where;
    where->prev->next = from;
    where->prev = to;

    return (to);
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, cmd_func_t cmd, void *param)
{
    dll_node_t *list_runner = (dll_node_t *)from;
    dll_node_t *to_node = (dll_node_t *)to;
    int status = 0;

    while (list_runner != to_node && 0 == status)
    {
        status = cmd((dlist_iter_t)list_runner->data, param);
        list_runner = list_runner->next;
    }

    return (status);
}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, comp_func_t comp,
                                                              const void *param)
{
    dll_node_t *list_runner = (dll_node_t *)from;
    dll_node_t *to_node = (dll_node_t *)to;
    int status = 0;

    assert(NULL != from);
    assert(NULL != to);

    while (list_runner != to_node->next && list_runner->next != NULL && 0 == status)
    {
        status = comp(list_runner->data, param);
        list_runner = list_runner->next;
    }
    if (0 == status)
    {
        return ((dlist_iter_t)to_node);
    }

    return ((dlist_iter_t)list_runner->prev);
}

dlist_iter_t DListNext(dlist_iter_t iter)
{
    dll_node_t *iter_node = (dll_node_t *)iter;

    if (NULL != iter_node->next)
    {
        iter_node = iter_node->next;
    }

    return ((dlist_iter_t)iter_node);
}

dlist_iter_t DListPrev(dlist_iter_t iter)
{
    dll_node_t *iter_node = (dll_node_t *)iter;

    if (NULL != iter_node->prev)
    {
        iter_node = iter_node->prev;
    }

    return ((dlist_iter_t)iter_node);
}

dlist_iter_t DListBegin(const dlist_t *dlist)
{
    assert(NULL != dlist);

    return ((dlist_iter_t)dlist->head->next);
}

dlist_iter_t DListEnd(const dlist_t *dlist)
{
    assert(NULL != dlist);

    return ((dlist_iter_t)dlist->tail);

}

dlist_iter_t DListInsert(dlist_t *dlist, dlist_iter_t iter, void *data)
{
    dll_node_t *iter_node = iter;
    dll_node_t *node = NULL;

    assert(NULL != dlist);
    assert(NULL != iter);
    assert(NULL != data);

    node = CreateNode(iter_node->prev, iter_node, data);
    if (NULL == node)
    {
        return (DListEnd(dlist));
    }

    iter_node->prev->next = node;
    iter_node->prev = node;

    return (node);
}

dlist_iter_t DListRemove(dlist_iter_t iter)
{
    dll_node_t *iter_next_backup = NULL;
    dll_node_t *iter_node = (dll_node_t *)iter;

    assert(NULL != iter);

    /*If iter is tail*/
    if (NULL == iter_node->next)
    {
        return ((dlist_iter_t)iter_node);
    }

    iter_next_backup = iter_node->next;
    iter_node->prev->next = iter_node->next;
    iter_node->next->prev = iter_node->prev;
    free(iter_node);

    return ((dlist_iter_t)iter_next_backup);
}

int DListIsSameIter(const dlist_iter_t iter1, const dlist_iter_t iter2)
{
    return (iter1 == iter2);
}

static dlist_iter_t DListInsertAfter(dlist_t *dlist, dlist_iter_t iter, void *data)
{
    dll_node_t *iter_node = iter;
    dll_node_t *node = NULL;

    assert(NULL != dlist);
    assert(NULL != iter);
    assert(NULL != data);

    if (iter == dlist->tail)
    {
        return (DListEnd(dlist));
    }

    node = CreateNode(iter_node, iter_node->next, data);
    if (NULL == node)
    {
        return (DListEnd(dlist));
    }

    iter_node->next->prev = node;
    iter_node->next = node;

    return ((dlist_iter_t)node);
}
