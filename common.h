#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

typedef int64_t mint;
#define PRIint PRId64
#define SCNint SCNd64
#define MINT_MAX INT64_MAX

typedef int8_t mints; // short integer
#define PRIints PRId8
#define SCNints SCNd8
#define MINTS_MAX INT8_MAX

typedef int64_t midx;
#define PRIidx PRId64
#define SCNidx SCNd64

#ifndef SORT_TEST_ITEM_COUNT // Don't redefine sample count if it is specified at cmd
#define SORT_TEST_ITEM_COUNT 10000 // denotes the number of samples in each sort test
#endif

//#define QUICK_SORT_VISUAL // toggles the visualization of the quick sort algorithm

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
#endif
