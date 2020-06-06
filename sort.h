#pragma once

#include "common.h"

typedef enum { SORT_TYPE_ASCENDING = 0, SORT_TYPE_DESCENDING = 1 } SortType;
i64 check_sort(i64 *arr, siz n,
               SortType type); // Checks whether an array is sorted

#define ADD_METHOD(x, ret)      \
	ret x(i64 *arr, siz count); \
	i64 test_##x();

ADD_METHOD(quick_sort, void)
ADD_METHOD(quick_sort_nonrec, void)

ADD_METHOD(merge_sort, void)
ADD_METHOD(merge_sort_nonrec, void)

ADD_METHOD(heap_sort, void)

ADD_METHOD(insertion_sort, void)
ADD_METHOD(insertion_sort_rec, void)

ADD_METHOD(selection_sort, void)
ADD_METHOD(selection_sort_rec, void)

ADD_METHOD(bubble_sort, void)
ADD_METHOD(bubble_sort_rec, void)
ADD_METHOD(bubble_sort_adv, void)

ADD_METHOD(radix_sort, void)

void test_sort();
