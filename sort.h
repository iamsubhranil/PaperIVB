#pragma once

#include "common.h"

#define ADD_METHOD(x, ret) \
    ret x(mint *arr, midx count); \
    mint test_##x();

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

void test_sort();
