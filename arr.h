#pragma once

#include <stdarg.h>
#include <stdlib.h>

#include "common.h"

#define arr_new(x) (mint *)malloc(sizeof(mint) * x)
#define arr_free(x) free(x)

void arr_fill(mint *arr, midx count, ...);
void arr_fill_int(mint *arr, midx count, ...); // To fill in the array with 32 bit integer values only

// SampleCase denotes the type of the generated
// list
//
// If SAMPLE_CASE_BEST is chosen, then the generated samples
// will already be sorted in ascending order
// If SAMPLE_CASE_WORST is chosen, then the generated samples
// will already be sorted in descending order
// If SAMPLE_CASE_AVERAGE is chosen, then the generated samples
// will be sorted in random order
typedef enum{
    SAMPLE_CASE_BEST,
    SAMPLE_CASE_AVERAGE,
    SAMPLE_CASE_WORST
} SampleCase;
void arr_fill_rand(mint *arr, midx count, mint range, SampleCase scase);

mint* arr_create();
void arr_print(mint *arr, midx n);
mint random_at_most(mint range);
mint* arr_copy(mint *arr, midx n);

void test_arr();
