#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "dlist.h"
#include "sorted_list.h"

#define UNUSED(expr) (void)(expr)

struct sorted_list_s
{
    dlist_t *dlist;
    is_before_func_t is_before;
    void *param;
};

static sorted_list_iter_t SortedListFindIfIsBefore(sorted_list_iter_t from,
                sorted_list_iter_t to, is_before_func_t is_before, const void *arg);


sorted_list_t *SortedListCreate(is_before_func_t is_before, void *param)
{
    sorted_list_t *sorted_list = malloc(sizeof(sorted_list_t));

    UNUSED(param);
    assert(NULL != is_before);

    if (NULL == sorted_list)
    {
        return (NULL);
    }

    sorted_list->dlist = DListCreate();
    if (NULL == sorted_list->dlist)
    {
        free(sorted_list);
        return (NULL);
    }

    sorted_list->is_before = is_before;
    sorted_list->param = param;

    return (sorted_list);
}

void SortedListDestroy(sorted_list_t *sorted_list)
{
    assert(NULL != sorted_list);

    DListDestroy(sorted_list->dlist);
    free(sorted_list);
}

size_t SortedListSize(const sorted_list_t *sorted_list)
{
    assert(NULL != sorted_list);

    return (DListSize(sorted_list->dlist));
}

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
    assert(NULL != sorted_list);

    return (DListIsEmpty(sorted_list->dlist));
}

sorted_list_iter_t SortedListBegin(sorted_list_t *sorted_list)
{
    sorted_list_iter_t s_iter;

    assert(NULL != sorted_list);

    s_iter.iter = DListBegin(sorted_list->dlist);

    return (s_iter);
}

sorted_list_iter_t SortedListEnd(sorted_list_t *sorted_list)
{
    sorted_list_iter_t sl_iter;

    assert(NULL != sorted_list);

    sl_iter.iter = DListEnd(sorted_list->dlist);

    return (sl_iter);
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t sl_iter)
{
    assert(sl_iter.iter);

    sl_iter.iter = DListNext(sl_iter.iter);

    return (sl_iter);
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t sl_iter)
{
    assert(sl_iter.iter);

    sl_iter.iter = DListPrev(sl_iter.iter);

    return (sl_iter);
}

int SortedListIsSame(const sorted_list_iter_t sl_iter1,
                                            const sorted_list_iter_t sl_iter2)
{
    return (sl_iter1.iter == sl_iter2.iter);
}

void *SortedListGetData(const sorted_list_iter_t sl_iter)
{
    assert(sl_iter.iter);

    return (DListGetData((dlist_iter_t)sl_iter.iter));
}

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
    sorted_list_iter_t sl_iter;
    dlist_iter_t runner = NULL;

    assert(sorted_list);

    runner = DListBegin(sorted_list->dlist);

    while (runner != DListEnd(sorted_list->dlist) &&
            (sorted_list->is_before(data, DListGetData(runner), sorted_list->param) == 0))
    {
        runner = DListNext(runner);
    }

    sl_iter.iter = DListInsert(sorted_list->dlist, runner, data);

    return (sl_iter);
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t sl_iter)
{
    assert(sl_iter.iter);

    if (NULL != SortedListNext(sl_iter).iter)
    {
        sl_iter.iter = DListRemove(sl_iter.iter);
    }

    return (sl_iter);
}

void *SortedListPopFront(sorted_list_t *sorted_list)
{
    void *data = NULL;

    assert(sorted_list);

    if (!SortedListIsEmpty(sorted_list))
    {
        data = DListGetData(DListBegin(sorted_list->dlist));
        DListPopFront(sorted_list->dlist);
    }

    return (data);
}

void *SortedListPopBack(sorted_list_t *sorted_list)
{
    void *data = NULL;

    assert(sorted_list);

    if (!SortedListIsEmpty(sorted_list))
    {
        data = DListGetData(DListPrev(DListEnd(sorted_list->dlist)));
        DListPopBack(sorted_list->dlist);
    }

    return (data);
}

int SortedListForEach(sorted_list_iter_t from,
    sorted_list_iter_t to, sorted_cmd_func_t cmd, void *arg)
{
    assert(from.iter);
    assert(to.iter);
    assert(cmd);

    return (DListForEach(from.iter, to.iter, (cmd_func_t)cmd, arg));
}

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
    sorted_list_iter_t where;
    sorted_list_iter_t from;
    sorted_list_iter_t to;
    sorted_list_iter_t src_end;
    sorted_list_iter_t dest_end;

    assert(dest);
    assert(src);

    where = SortedListBegin(dest);
    from = SortedListBegin(src);
    to = SortedListBegin(src);
    src_end = SortedListEnd(src);
    dest_end = SortedListEnd(dest);

    while (!SortedListIsSame(from, src_end))
    {
        while (!SortedListIsSame(dest_end, where) &&
            dest->is_before(SortedListGetData(where), SortedListGetData(from), NULL))
        {
            where = SortedListNext(where);
        }

        if(SortedListIsSame(where, dest_end))
        {
            to = src_end;
        }
        else
        {
            while (0 == dest->is_before(SortedListGetData(where), SortedListGetData(to), NULL))
            {
                to = SortedListNext(to);
            }
        }

        DListSplice(from.iter, DListPrev(to.iter), where.iter);
        from = to;
    }
}


sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list,
    sorted_list_iter_t from, sorted_list_iter_t to, const void *arg)
{
    assert(sorted_list);
    assert(from.iter);
    assert(to.iter);

    return (SortedListFindIfIsBefore(from, to, sorted_list->is_before, arg));
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from,
                                    sorted_list_iter_t to,
                                    int (*is_match)(const void *data, const void *param),
                                    const void *arg)
{
    sorted_list_iter_t sl_iter;

    assert(from.iter);
    assert(to.iter);

    sl_iter.iter = DListFind(from.iter, to.iter, is_match, arg);

    return (sl_iter);
}

static sorted_list_iter_t SortedListFindIfIsBefore(sorted_list_iter_t from,
                sorted_list_iter_t to, is_before_func_t is_before, const void *arg)
{
    sorted_list_iter_t sl_iter;
    dlist_iter_t runner = NULL;
    int is_before_res = 1;

    assert(from.iter);
    assert(to.iter);

    runner = from.iter;

    while (runner != to.iter && 0 != is_before_res)
    {
        is_before_res = is_before(DListGetData(runner), arg, NULL);
        runner = DListNext(runner);
    }

    if (0 == is_before(arg, DListGetData(DListPrev(runner)), NULL))
    {
        sl_iter.iter = DListPrev(runner);
        return (sl_iter);
    }

    return (to);
}
