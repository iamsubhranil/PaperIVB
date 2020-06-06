#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int64_t i64;
#define Pi64 PRId64
#define Si64 SCNd64
#define i64_MAX INT64_MAX
#define i64_MIN INT64_MIN

typedef uint64_t u64;
#define Pu64 PRIu64
#define Su64 SCNu64
#define u64_MAX UINT64_MAX
#define u64_MIN UINT64_MIN

typedef int32_t i32;
#define Pi32 PRId32
#define Si32 SCNd32
#define i32_MAX INT32_MAX
#define i32_MIN INT32_MIN

typedef uint32_t u32;
#define Pu32 PRIu32
#define Su32 SCNu32
#define u32_MAX UINT32_MAX
#define u32_MIN UINT32_MIN

typedef int16_t i16;
#define Pi16 PRId16
#define Si16 SCNd16
#define i16_MAX INT16_MAX
#define i16_MIN INT16_MIN

typedef uint16_t u16;
#define Pu16 PRIu16
#define Su16 SCNu16
#define u16_MAX UINT16_MAX
#define u16_MIN UINT16_MIN

typedef int8_t i8;
#define Pi8 PRId8
#define Si8 SCNd8
#define i8_MAX INT8_MAX
#define i8_MIN INT8_MIN

typedef uint8_t u8;
#define Pu8 PRIu8
#define Su8 SCNu8
#define u8_MAX UINT8_MAX
#define u8_MIN UINT8_MIN

typedef size_t siz;
#define Psiz "zd"
#define Ssiz "zd"

#define STACKTRACE_SHOW

#ifdef __ANDROID__ // no execinfo.h
#undef STACKTRACE_SHOW
#endif

// macros
#ifndef ARRAY_TEST_ITEM_COUNT
#define ARRAY_TEST_ITEM_COUNT 10000
#endif

#ifndef AVL_TEST_ITEM_COUNT
#define AVL_TEST_ITEM_COUNT 10000
#endif

#ifndef BST_TEST_ITEM_COUNT
#define BST_TEST_ITEM_COUNT 10000
#endif

#ifndef HASHTABLE_TEST_ITEM_COUNT
#define HASHTABLE_TEST_ITEM_COUNT 10000
#endif

#ifndef QUEUE_TEST_ITEM_COUNT
#define QUEUE_TEST_ITEM_COUNT 10000
#endif

#ifdef QUEUE_ENABLE_FAST         // Enable fixed-front queue (queue_fast.c)
#elif defined(QUEUE_ENABLE_LINK) // Enable linked queue (queue_linked.c)
#else
#ifndef QUEUE_ENABLE_CIRCULAR // Default circular queue (queue.c)
#define QUEUE_ENABLE_CIRCULAR
#endif
#endif

#ifndef SORT_TEST_ITEM_COUNT // Don't redefine sample count if it is specified
                             // at cmd
#define SORT_TEST_ITEM_COUNT \
	10000 // denotes the number of samples in each sort test
#endif

//#define SORT_ENABLE_VISUAL // toggles the visualization of all algorithms

#ifndef STACK_TEST_ITEM_COUNT
#define STACK_TEST_ITEM_COUNT 10000
#endif

typedef struct {
	union {
		struct {
			i64 arr_test_item_count;
			i64 avl_test_item_count;
			i64 bst_test_item_count;
			i64 graph_test_item_count; // unused
			i64 hashtable_test_item_count;
			i64 numerical_test_item_count; // unused
			i64 queue_test_item_count;
			i64 sort_test_item_count;
			i64 stack_test_item_count;
		};
		i64 counts[9];
	};
	union {
		struct {
			bool arr_test;
			bool avl_test;
			bool bst_test;
			bool graph_test;
			bool hashtable_test;
			bool numerical_test;
			bool queue_test;
			bool sort_test;
			bool stack_test;
		};
		bool tests[9];
	};
	bool graph_test_floyd_warshall;
	union {
		struct {
			bool sort_test_bubble;
			bool sort_test_bubble_rec;
			bool sort_test_bubble_adv;
			bool sort_test_heap;
			bool sort_test_insertion;
			bool sort_test_insertion_rec;
			bool sort_test_merge;
			bool sort_test_merge_nonrec;
			bool sort_test_quick;
			bool sort_test_quick_nonrec;
			bool sort_test_radix;
			bool sort_test_selection;
			bool sort_test_selection_rec;
		};
		bool sort_tests[13];
	};
#ifdef SORT_ENABLE_VISUAL
	bool sort_enable_visual;
#endif
} Configuration;

extern Configuration config;
