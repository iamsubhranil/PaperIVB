#pragma once

#include "common.h"

typedef enum{
    SORT_TYPE_ASCENDING = 0,
    SORT_TYPE_DESCENDING = 1
} SortType;
mint check_sort(mint *arr, midx n, SortType type); // Checks whether an array is sorted

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

ADD_METHOD(radix_sort, void)

void test_sort();

#ifndef SORT_TEST_ITEM_COUNT // Don't redefine sample count if it is specified at cmd
#define SORT_TEST_ITEM_COUNT 10000 // denotes the number of samples in each sort test
#endif

//#define SORT_ENABLE_VISUAL // toggles the visualization of all algorithms

#ifdef SORT_ENABLE_TEST_ALL
#define SORT_ENABLE_TEST_BUBBLE
#define SORT_ENABLE_TEST_BUBBLE_REC
#define SORT_ENABLE_TEST_BUBBLE_ADV
#define SORT_ENABLE_TEST_SELECTION
#define SORT_ENABLE_TEST_SELECTION_REC
#define SORT_ENABLE_TEST_INSERTION
#define SORT_ENABLE_TEST_INSERTION_REC
#define SORT_ENABLE_TEST_QUICK
#define SORT_ENABLE_TEST_QUICK_NONREC
#define SORT_ENABLE_TEST_HEAP
#define SORT_ENABLE_TEST_MERGE
#define SORT_ENABLE_TEST_MERGE_NONREC
#define SORT_ENABLE_TEST_RADIX
#endif
