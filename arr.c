#include "arr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "display.h"
#include "test.h"
#include "utils.h"

void arr_fill_int(i64 *arr, siz count, ...) {
	va_list list;
	va_start(list, count);
	for(siz i = 0; i < count; i++) {
		// Default int size is 32, which is also
		// default integer argument size. Casting it to
		// i64 aka int64_t introduces some nasty
		// size mismatch bug at the multiple of 5,
		// producing garbage values as a result of
		// casting
		//
		// One remedy should be explicitly casting each
		// argument to i64 before/while passing.
		// The other is passing only integer elements,
		// while the cast will only be `int`.
		//
		// Hence, two separate methods are given to
		// justify these causes.
		i64 el = va_arg(list, int);

		arr[i] = el;
	}
	va_end(list);
}

void arr_fill_rand(i64 *arr, siz count, i64 range, SampleCase scase) {
	srand(time(NULL));
	i64 start = random_at_most(range);
	while((scase == SAMPLE_CASE_BEST && (siz)(i64_MAX - start) < count) ||
	      (scase == SAMPLE_CASE_WORST && (siz)(i64_MAX + start) < count))
		start = random_at_most(range);

	for(siz i = 0; i < count; i++) {
		arr[i] = scase == SAMPLE_CASE_AVERAGE
		             ? random_at_most(range)
		             : scase == SAMPLE_CASE_BEST ? start++ : start--;
	}
}

void arr_fill(i64 *arr, siz count, ...) {
	va_list list;
	va_start(list, count);
	for(siz i = 0; i < count; i++) {
		i64 el = va_arg(list, i64);
		arr[i] = el;
	}
	va_end(list);
}

i64 *arr_create() {
reinput:
	pgrn(ANSI_COLOR_GREEN ANSI_FONT_BOLD "[Input] " ANSI_COLOR_RESET);
	info("Size of the array : ");
	siz size;
	scanf("%" Ssiz, &size);
	if(size < 1) {
		err("Size must be >= 1!");
		goto reinput;
	}
	i64 *arr = arr_new(size);
	if(!arr) {
		err("Array can not be initialized!");
		exit(1);
	}
	for(siz i = 0; i < size; i++) {
		pblue(ANSI_FONT_BOLD ANSI_COLOR_BLUE "[Element %" Psiz
		                                     "] " ANSI_COLOR_RESET,
		      i + 1);
		scanf("%" Si64, &arr[i]);
	}
	return arr;
}

void arr_print(i64 *arr, siz n) {
	printf("{ ");
	if(n == 0) {
		printf("Empty }");
		return;
	}
	printf("%" Pi64, arr[0]);
	for(siz i = 1; i < n; i++) printf(", %" Pi64, arr[i]);
	printf(" }");
}

i64 *arr_copy(i64 *source, siz n) {
	i64 *dest = arr_new(n);
	memcpy(dest, source, sizeof(i64) * n);
	return dest;
}

static i64 test_arr_fill(i64 *arr) {
	arr_fill_int(arr, 2, 2, 3);
	if(arr[1] != 3) {
		return 0;
	}
	arr_fill_int(arr, 5, 8, 2, 34, 2, 1);
	if(arr[4] != 1) {
		return 0;
	}
	if(arr[0] != 8) {
		return 0;
	}
	return 1;
}

static i64 test_random_at_most() {
	for(i64 i = 0; i < 1000; i++)
		if(random_at_most(100) > 100)
			return 0;
	for(i64 i = 0; i < random_at_most(1000); i++) {
		i64 range = random_at_most(93828);
		if(random_at_most(range) > range)
			return 0;
	}
	return 1;
}

static i64 test_arr_copy(i64 *source, siz n) {
	arr_fill_rand(source, config.arr_test_item_count,
	              config.arr_test_item_count, SAMPLE_CASE_AVERAGE);
	i64 *dest = arr_copy(source, n);
	for(siz i = 0; i < n; i++) {
		if(source[i] != dest[i]) {
			arr_free(dest);
			return 0;
		}
	}
	arr_free(dest);
	return 1;
}

void test_arr() {
	i64 *arr = arr_new(config.arr_test_item_count);
	tst_suite_start("Array", 3);
	TEST("Create", arr);
	TEST("Fill", test_arr_fill(arr));
	TEST("Copy", test_arr_copy(arr, config.arr_test_item_count));
	arr_free(arr);
	tst_suite_end();
	TEST("Random In Range", test_random_at_most());
}
