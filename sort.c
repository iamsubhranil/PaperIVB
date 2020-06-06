#include "sort.h"

#include "arr.h"
#include "display.h"
#include "dump.h"
#include "queue.h"
#include "stack.h"
#include "test.h"

#ifdef SORT_ENABLE_VISUAL

#include "histogram.h"

#endif

i64 check_sort(i64 *arr, siz n, SortType type) {
	if(type == SORT_TYPE_ASCENDING) {
		for(siz i = 0; i < n - 1; i++) {
			if(arr[i] > arr[i + 1]) {
				dbg("arr[%" Pi64 "] : %" Pi64 "\t arr[%" Pi64 "] : %" Pi64 "\n",
				    i, arr[i], i + 1, arr[i + 1]);
				return 0;
			}
		}
	} else {
		for(siz i = 0; i < n - 1; i++) {
			if(arr[i] < arr[i + 1]) {
				dbg("arr[%" Pi64 "] : %" Pi64 "\t arr[%" Pi64 "] : %" Pi64 "\n",
				    i, arr[i], i + 1, arr[i + 1]);
				return 0;
			}
		}
	}
	return 1;
}

#ifdef SORT_ENABLE_VISUAL
#define SORT_CUST_RANGE config.sort_enable_visual ? 20 : (rand() + 1)
#else
#define SORT_CUST_RANGE (rand() + 1)
#endif

#define sort_test(name)                                                 \
	i64 test_##name() {                                                 \
		i64 size = config.sort_test_item_count;                         \
		tst_pause("Creating array");                                    \
		i64 *arr = arr_new(size);                                       \
		tst_pause("Generating random input : Best Case");               \
		arr_fill_rand(arr, size, size, SAMPLE_CASE_BEST);               \
		dump_set(arr, size);                                            \
		tst_resume("Best Case");                                        \
		name(arr, size);                                                \
		tst_pause("Checking result");                                   \
		if(!check_sort(arr, size, SORT_TYPE_ASCENDING)) {               \
			goto _test_fail_##name;                                     \
		}                                                               \
		tst_pause("Generating random input : Worst Case");              \
		arr_fill_rand(arr, size, -size, SAMPLE_CASE_WORST);             \
		dump_set(arr, size);                                            \
		tst_resume("Worst Case");                                       \
		name(arr, size);                                                \
		tst_pause("Checking result");                                   \
		if(!check_sort(arr, size, SORT_TYPE_ASCENDING)) {               \
			goto _test_fail_##name;                                     \
		}                                                               \
		tst_pause("Generating random input : Average Case");            \
		arr_fill_rand(arr, size, SORT_CUST_RANGE, SAMPLE_CASE_AVERAGE); \
		dump_set(arr, size);                                            \
		tst_resume("Average Case");                                     \
		name(arr, size);                                                \
		tst_pause("Checking result");                                   \
		if(!check_sort(arr, size, SORT_TYPE_ASCENDING)) {               \
			goto _test_fail_##name;                                     \
		}                                                               \
		goto _test_done_##name;                                         \
		_test_fail_##name : dump_data(#name "_fail");                   \
		tst_resume("Cleanup");                                          \
		free(arr);                                                      \
		return 0;                                                       \
		_test_done_##name : tst_resume("Cleanup");                      \
		free(arr);                                                      \
		return 1;                                                       \
	}

#define swap(p1, p2) \
	{                \
		i64 p3 = p1; \
		p1     = p2; \
		p2     = p3; \
	}

void bubble_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Bubble Sort");
#endif
	for(siz pass = 0; pass < n; pass++) {
		for(siz i = 0; i < n - pass - 1; i++) {
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, n, 2, pass, ANSI_COLOR_BLUE, i,
				           ANSI_COLOR_GREEN);
#endif
			if(arr[i] > arr[i + 1]) {
				swap(arr[i], arr[i + 1]);
			}
		}
	}
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_draw(arr, n, 0);
#endif
}

sort_test(bubble_sort);

static void bubble_sort_rec2(i64 *arr, siz pass, siz n) {
	if(pass == n)
		return;
	for(siz i = 0; i < n - pass - 1; i++) {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, n, 2, pass, ANSI_COLOR_BLUE, i, ANSI_COLOR_GREEN);
#endif
		if(arr[i] > arr[i + 1]) {
			swap(arr[i], arr[i + 1]);
		}
	}
	bubble_sort_rec2(arr, pass + 1, n);
}

void bubble_sort_rec(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Bubble Sort Recursive");
#endif
	bubble_sort_rec2(arr, 0, n);
}

sort_test(bubble_sort_rec);

void bubble_sort_adv(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Bubble Sort Advanced");
#endif
	for(siz pass = 0; pass < n; pass++) {
		i64 flag = 0;
		for(siz i = 0; i < n - pass - 1; i++) {
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, n, 2, pass, ANSI_COLOR_BLUE, i,
				           ANSI_COLOR_GREEN);
#endif
			if(arr[i] > arr[i + 1]) {
				swap(arr[i], arr[i + 1]);
				flag = 1;
			}
		}
		if(!flag)
			return;
	}
}

sort_test(bubble_sort_adv);

void insertion_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Insertion Sort");
#endif
	for(siz i = 0; i < n; i++) {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, n, 1, i, ANSI_COLOR_GREEN);
#endif
		i64 k = arr[i];
		i64 j = i - 1;
		while(j >= 0 && k < arr[j]) {
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, n, 2, i, ANSI_COLOR_GREEN, j, ANSI_COLOR_BLUE,
				           k, ANSI_COLOR_RED);
#endif
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = k;
	}
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_draw(arr, n, 0);
#endif
}

sort_test(insertion_sort);

static void insertion_sort_rec2(i64 *arr, siz i, siz n) {
	if(i == n)
		return;
	i64 k = arr[i];
	i64 j = i - 1;
	while(j >= 0 && k < arr[j]) {
		arr[j + 1] = arr[j];
		j--;
	}
	arr[j + 1] = k;
	insertion_sort_rec2(arr, i + 1, n);
}

void insertion_sort_rec(i64 *arr, siz n) {
	insertion_sort_rec2(arr, 0, n);
}

sort_test(insertion_sort_rec);

void selection_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Selection Sort");
#endif
	for(siz i = 0; i < n; i++) {
		siz minidx = i;
		for(siz j = i + 1; j < n; j++) {
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, n, 2, i, ANSI_COLOR_GREEN, j, ANSI_COLOR_BLUE);
#endif
			if(arr[j] < arr[minidx]) {
				minidx = j;
			}
		}
		if(minidx != i)
			swap(arr[i], arr[minidx]);
	}
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_draw(arr, n, 0);
#endif
}

sort_test(selection_sort);

static void selection_sort_rec2(i64 *arr, siz i, siz n) {
	if(i == n)
		return;
	siz minidx = i;
	for(siz j = i + 1; j < n; j++) {
		if(arr[j] < arr[minidx])
			minidx = j;
	}
	if(minidx != i)
		swap(arr[i], arr[minidx]);
	selection_sort_rec2(arr, i + 1, n);
}

void selection_sort_rec(i64 *arr, siz n) {
	selection_sort_rec2(arr, 0, n);
}

sort_test(selection_sort_rec);

static siz quick_sort_partition(i64 *arr, siz m, siz n, siz total) {
	i64 pivot = arr[m];
	siz i     = m;
	siz j     = n + 1;
	do {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN,
			           m, ANSI_COLOR_RED);
#endif
		do {
			i++;
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j,
				           ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
		} while(i < total - 1 && arr[i] < pivot);
		do {
			j--;
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j,
				           ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
		} while(arr[j] > pivot && j > m);
		if(i < j) {
			swap(arr[i], arr[j]);
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j,
				           ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
		}
	} while(i < j);
	if(j != m)
		swap(arr[m], arr[j]);
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN, m,
		           ANSI_COLOR_RED);
#endif
	return j;
}

void quick_sort2(i64 *arr, siz m, siz n, siz total) {
	if(m >= n)
		return;
	siz j = quick_sort_partition(arr, m, n, total);
	if(j != 0)
		quick_sort2(arr, m, j - 1, total);
	quick_sort2(arr, j + 1, n, total);
}

void quick_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Quick Sort");
#endif
	quick_sort2(arr, 0, n - 1, n);
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_draw(arr, n, 0);
#endif
}

sort_test(quick_sort);

void quick_sort_nonrec(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Quick Sort Nonrecursive");
#endif
	Stack start = stack_new(n / 2 + 1, 0);
	Stack stop  = stack_new(n / 2 + 1, 0);
	stack_push_fast(start, 0);
	stack_push_fast(stop, n - 1);
	siz total = n;
	while(!stack_is_empty(start)) {
		siz m = stack_pop_fast(start);
		n     = stack_pop_fast(stop);
		siz j = quick_sort_partition(arr, m, n, total);
		if(j != 0 && m < (j - 1)) {
			stack_push_fast(start, m);
			stack_push_fast(stop, j - 1);
		}
		if(j + 1 < n) {
			stack_push_fast(start, j + 1);
			stack_push_fast(stop, n);
		}
	}
	stack_free(start);
	stack_free(stop);
}

sort_test(quick_sort_nonrec);

#ifdef SORT_ENABLE_VISUAL
static void sorted_merge(i64 *arr, siz l, siz m, siz u, i64 *aux, i64 total) {
#else
static void sorted_merge(i64 *arr, siz l, siz m, siz u, i64 *aux) {
#endif
	siz pointer = 0, mid = m + 1, low = l;
	while(low <= m && mid <= u) {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, m, ANSI_COLOR_RED,
			           mid, ANSI_COLOR_GREEN);
#endif
		if(arr[low] > arr[mid]) {
			aux[pointer] = arr[mid];
			mid++;
		} else {
			aux[pointer] = arr[low];
			low++;
		}
		pointer++;
	}
	while(low <= m) {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, m, ANSI_COLOR_RED,
			           mid, ANSI_COLOR_GREEN);
#endif
		aux[pointer] = arr[low];
		low++;
		pointer++;
	}
	while(mid <= u) {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, m, ANSI_COLOR_RED,
			           mid, ANSI_COLOR_GREEN);
#endif
		aux[pointer] = arr[mid];
		mid++;
		pointer++;
	}
	for(siz i = 0; i < pointer; i++) {
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, i + l,
			           ANSI_COLOR_RED, mid, ANSI_COLOR_GREEN);
#endif
		arr[i + l] = aux[i];
	}
}

#ifdef SORT_ENABLE_VISUAL
static void merge_sort2(i64 *arr, siz l, siz u, i64 *aux, i64 total) {
#else
static void merge_sort2(i64 *arr, siz l, siz u, i64 *aux) {
#endif
	if(l >= u)
		return;
	siz m = (l + u) / 2;
#ifdef SORT_ENABLE_VISUAL
	merge_sort2(arr, l, m, aux, total);
	merge_sort2(arr, m + 1, u, aux, total);
	sorted_merge(arr, l, m, u, aux, total);
#else
	merge_sort2(arr, l, m, aux);
	merge_sort2(arr, m + 1, u, aux);
	sorted_merge(arr, l, m, u, aux);
#endif
}

void merge_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Merge Sort");
#endif
	i64 *aux = arr_new(n);
#ifdef SORT_ENABLE_VISUAL
	merge_sort2(arr, 0, n - 1, aux, n);
	if(config.sort_enable_visual)
		histo_draw(arr, n, 0);
#else
	merge_sort2(arr, 0, n - 1, aux);
#endif
	arr_free(aux);
}

sort_test(merge_sort);

void merge_sort_nonrec(i64 *arr, siz n) {
	Stack start = stack_new(n, 0);
	Stack stop  = stack_new(n, 0);
	Stack beg   = stack_new(n, 0);
	Stack end   = stack_new(n, 0);
	Stack mid   = stack_new(n, 0);

	i64 *aux = arr_new(n);

	stack_push_fast(start, 0);
	stack_push_fast(stop, n - 1);
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Merge Sort Nonrecursive");
#endif
	while(!stack_is_empty(start)) {
		siz s = stack_pop_fast(start);
		siz e = stack_pop_fast(stop);
		siz m = (s + e) / 2;

		stack_push_fast(beg, s);
		stack_push_fast(end, e);
		stack_push_fast(mid, m);

		if(s < m) {
			stack_push_fast(start, s);
			stack_push_fast(stop, m);
		}
		if(m + 1 < e) {
			stack_push_fast(start, m + 1);
			stack_push_fast(stop, e);
		}
	}
	while(!stack_is_empty(beg)) {
		siz b = stack_pop_fast(beg);
		siz m = stack_pop_fast(mid);
		siz e = stack_pop_fast(end);
#ifdef SORT_ENABLE_VISUAL
		sorted_merge(arr, b, m, e, aux, n);
#else
		sorted_merge(arr, b, m, e, aux);
#endif
	}

	arr_free(aux);
	stack_free(start);
	stack_free(stop);
	stack_free(beg);
	stack_free(mid);
	stack_free(end);
}

sort_test(merge_sort_nonrec);

static i64 *heap_create(i64 *arr, siz n) {
	i64 *heap = arr_new(n);
	for(siz i = 0; i < n; i++) {
		heap[i] = arr[i];
		siz j   = i;
		while(j > 0) {
			siz parent = (j + 1) / 2 - 1;
#ifdef SORT_ENABLE_VISUAL
			if(config.sort_enable_visual)
				histo_draw(arr, n, 2, j, ANSI_COLOR_BLUE, parent,
				           ANSI_COLOR_RED);
#endif
			// Min heap
			if(heap[j] < heap[parent]) {
				swap(heap[j], heap[parent]);
				j = parent;
				continue;
			}
			break;
		}
	}
	return heap;
}

#ifdef SORT_ENABLE_VISUAL
static void heap_rebuild(i64 *arr, siz n, siz total) {
#else
static void heap_rebuild(i64 *arr, siz n) {
#endif
	if(n == 1)
		return;
	siz j = 0;
	while(1) {
		siz leftChild = (j * 2) + 1, rightChild = (j * 2) + 2;
		siz min = j;
#ifdef SORT_ENABLE_VISUAL
		if(config.sort_enable_visual)
			histo_draw(arr, total, 3, leftChild, ANSI_COLOR_BLUE, j,
			           ANSI_COLOR_RED, rightChild, ANSI_COLOR_GREEN);
#endif
		if(leftChild < n && arr[leftChild] < arr[min]) {
			min = leftChild;
		}
		if(rightChild < n && arr[rightChild] < arr[min]) {
			min = rightChild;
		}
		if(min != j) {
			swap(arr[min], arr[j]);
			j = min;
			continue;
		}
		break;
	}
}

void heap_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Heap Sort");
#endif
	i64 *heap = heap_create(arr, n);
	for(siz i = n, j = 0; i > 0; i--, j++) {
		arr[j]  = heap[0];
		heap[0] = heap[i - 1];
#ifdef SORT_ENABLE_VISUAL
		heap_rebuild(heap, i - 1, n);
#else
		heap_rebuild(heap, i - 1);
#endif
	}
	arr[n - 1] = heap[0];
	arr_free(heap);
}

sort_test(heap_sort);

void radix_sort(i64 *arr, siz n) {
#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_set_title("Radix Sort");
#endif
	i64 max = arr[0], min = arr[0];
	for(siz i = 1; i < n; i++) {
		if(arr[i] > max)
			max = arr[i];
		if(arr[i] < min)
			min = arr[i];
	}

	min = min < 0 ? -min : min;
	if(min > max)
		max = min;

	i64 passes = 1;
	if(max < 0)
		max = -max;
	while(max > 0) {
		passes++;
		max /= 10;
	}

	Queue queue[20];
	for(i64 i = 0; i < 20; i++) queue[i] = queue_new(n);

	i64 pwr = 1;
	for(i64 i = 0; i < passes; i++) {
		for(siz j = 0; j < n; j++) {
			i64 tmp    = arr[j] < 0 ? -arr[j] : arr[j];
			i64 bucket = (tmp / pwr) % 10;
			if(arr[j] >= 0)
				bucket += 10;
			else {
				bucket = 9 - bucket;
			}
			queue_insert(queue[bucket], arr[j]);
		}

		i64 bucket = 0;

		for(siz j = 0; j < n;) {
			while(queue_is_empty(queue[bucket])) bucket++;
			while(!queue_is_empty(queue[bucket])) {
#ifdef SORT_ENABLE_VISUAL
				if(config.sort_enable_visual)
					histo_draw(arr, n, 1, j, ANSI_COLOR_BLUE);
#endif
				arr[j] = queue_delete(queue[bucket]);
				j++;
			}
			bucket++;
		}
		pwr *= 10;
	}

	for(i64 i = 0; i < 20; i++) queue_free(queue[i]);

#ifdef SORT_ENABLE_VISUAL
	if(config.sort_enable_visual)
		histo_draw(arr, n, 0);
#endif
}

sort_test(radix_sort);

void test_sort() {
	u64 test_count = 0;
	if(config.sort_test_bubble)
		test_count++;

	if(config.sort_test_bubble_rec)
		test_count++;

	if(config.sort_test_bubble_adv)
		test_count++;

	if(config.sort_test_insertion)
		test_count++;

	if(config.sort_test_insertion_rec)
		test_count++;

	if(config.sort_test_quick)
		test_count++;

	if(config.sort_test_quick_nonrec)
		test_count++;

	if(config.sort_test_heap)
		test_count++;

	if(config.sort_test_merge)
		test_count++;

	if(config.sort_test_merge_nonrec)
		test_count++;

	if(config.sort_test_radix)
		test_count++;

	if(config.sort_test_selection)
		test_count++;

	if(config.sort_test_selection_rec)
		test_count++;

	if(test_count > 0)
		tst_suite_start("Sorting", test_count);

	if(config.sort_test_bubble)
		TEST("Bubble Sort", test_bubble_sort());

	if(config.sort_test_bubble_rec)
		TEST("Bubble Sort Recursive", test_bubble_sort_rec());

	if(config.sort_test_bubble_adv)
		TEST("Bubble Sort Advanced", test_bubble_sort_adv());

	if(config.sort_test_insertion)
		TEST("Insertion Sort", test_insertion_sort());

	if(config.sort_test_insertion_rec)
		TEST("Insertion Sort Recursive", test_insertion_sort_rec());

	if(config.sort_test_selection)
		TEST("Selection Sort", test_selection_sort());

	if(config.sort_test_selection_rec)
		TEST("Selection Sort Recursive", test_selection_sort_rec());

	if(config.sort_test_quick)
		TEST("Quick Sort", test_quick_sort());

	if(config.sort_test_quick_nonrec)
		TEST("Quick Sort Nonrecursive", test_quick_sort_nonrec());

	if(config.sort_test_merge)
		TEST("Merge Sort", test_merge_sort());

	if(config.sort_test_merge_nonrec)
		TEST("Merge Sort Nonrecursive", test_merge_sort_nonrec());

	if(config.sort_test_heap)
		TEST("Heap Sort", test_heap_sort());

	if(config.sort_test_radix)
		TEST("Radix Sort", test_radix_sort());

	if(test_count > 0)
		tst_suite_end();
}
