#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

typedef int64_t mint;
#define PRIint PRId64
#define SCNint SCNd64
#define MINT_MAX INT64_MAX

typedef int64_t midx;
#define PRIidx PRId64
#define SCNidx SCNd64

#define SORT_TEST_ITEM_COUNT 10000 // denotes the number of samples in each sort test
//#define QUICK_SORT_VISUAL // toggles the visualization of the quick sort algorithm
