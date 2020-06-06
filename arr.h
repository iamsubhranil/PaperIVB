#pragma once

#include <stdarg.h>
#include <stdlib.h>

#include "common.h"

#define arr_new(x) (i64 *)malloc(sizeof(i64) * x)
#define arr_free(x) free(x)

void arr_fill(i64 *arr, siz count, ...);
void arr_fill_int(i64 *arr, siz count,
                  ...); // To fill in the array with 32 bit integer values only

// SampleCase denotes the type of the generated
// list
//
// If SAMPLE_CASE_BEST is chosen, then the generated samples
// will already be sorted in ascending order
// If SAMPLE_CASE_WORST is chosen, then the generated samples
// will already be sorted in descending order
// If SAMPLE_CASE_AVERAGE is chosen, then the generated samples
// will be sorted in random order
typedef enum {
	SAMPLE_CASE_BEST,
	SAMPLE_CASE_AVERAGE,
	SAMPLE_CASE_WORST
} SampleCase;
void arr_fill_rand(i64 *arr, siz count, i64 range, SampleCase scase);

i64 *arr_create();
void arr_print(i64 *arr, siz n);
i64 *arr_copy(i64 *arr, siz n);

void test_arr();
