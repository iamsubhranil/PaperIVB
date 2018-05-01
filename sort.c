#include "sort.h"
#include "arr.h"
#include "test.h"
#include "display.h"
#include "stack.h"

static mint check_sort(mint *arr, midx n){
    for(midx i = 0;i < n-1;i++){
        if(arr[i] > arr[i+1]){
            dbg("arr[%" PRIint "] : %" PRIint "\t arr[%" PRIint "] : %" PRIint "\n", i, arr[i], i+1, arr[i+1]);
            return 0;
        }
    }
    return 1;
}

#define sort_test(name, sz, customrange) \
    mint test_##name(){ \
        mint size = sz; \
        tst_pause("Creating array"); \
        mint *arr = arr_new(size); \
        tst_pause("Generating random input : Best Case"); \
        arr_fill_rand(arr, size, size, SAMPLE_CASE_BEST); \
        tst_resume("Best Case"); \
        name(arr, size); \
        tst_pause("Checking result"); \
        if(!check_sort(arr, size)){ \
            goto _test_fail_##name; \
        } \
        tst_pause("Generating random input : Worst Case"); \
        arr_fill_rand(arr, size, -size, SAMPLE_CASE_WORST); \
        tst_resume("Worst Case"); \
        name(arr, size); \
        tst_pause("Checking result"); \
        if(!check_sort(arr, size)){ \
            goto _test_fail_##name; \
        } \
        tst_pause("Generating random input : Average Case"); \
        arr_fill_rand(arr, size, random_at_most(customrange), SAMPLE_CASE_AVERAGE); \
        tst_resume("Average Case"); \
        name(arr, size); \
        tst_pause("Checking result"); \
        if(!check_sort(arr, size)) { \
            goto _test_fail_##name; \
        } \
        goto _test_done_##name; \
        _test_fail_##name: \
        tst_resume("Cleanup"); \
        free(arr); \
        return 0; \
        _test_done_##name: \
        tst_resume("Cleanup"); \
        free(arr); \
        return 1; \
    }

static void swap(mint *p1, mint *p2){
    mint p3 = *p1;
    *p1 = *p2;
    *p2 = p3;
}

void bubble_sort(mint *arr, midx n){
    for(midx pass = 0;pass < n;pass++){
        for(midx i = 0;i < n-pass-1;i++){
            if(arr[i] > arr[i+1]){
                swap(&arr[i], &arr[i+1]);
            }
        }
    }
}

sort_test(bubble_sort, SORT_TEST_ITEM_COUNT, 372);

static void bubble_sort_rec2(mint *arr, midx pass, midx n){
    if(pass == n)
        return;
    for(midx i = 0;i < n-pass-1;i++){
        if(arr[i] > arr[i+1]){
            swap(&arr[i], &arr[i+1]);
        }
    }
    bubble_sort_rec2(arr, pass+1, n);
}

void bubble_sort_rec(mint *arr, midx n){
    bubble_sort_rec2(arr, 0, n);
}

sort_test(bubble_sort_rec, SORT_TEST_ITEM_COUNT, 483);

void bubble_sort_adv(mint *arr, midx n){
    for(midx pass = 0;pass < n;pass++){
        mint flag = 0;
        for(midx i = 0;i < n - pass - 1;i++){
            if(arr[i] > arr[i+1]){
                swap(&arr[i], &arr[i+1]);
                flag = 1;
            }
        }
        if(!flag)
            return;
    }
}

sort_test(bubble_sort_adv, SORT_TEST_ITEM_COUNT, 281);

void insertion_sort(mint *arr, midx n){
    for(midx i = 0; i < n; i++){
        mint k = arr[i];
        midx j = i - 1;
        while(j >= 0 && k < arr[j]){
            arr[j+1] = arr[j];
            j--;
        }
        arr[j + 1] = k;
    }
}

sort_test(insertion_sort, SORT_TEST_ITEM_COUNT, 3478387);

static void insertion_sort_rec2(mint *arr, midx i, midx n){
    if(i == n)
        return;
    mint k = arr[i];
    midx j = i - 1;
    while(j >= 0 && k < arr[j]){
        arr[j+1] = arr[j];
        j--;
    }
    arr[j+1] = k;
    insertion_sort_rec2(arr, i+1, n);
}

void insertion_sort_rec(mint *arr, midx n){
    insertion_sort_rec2(arr, 0, n);
}

sort_test(insertion_sort_rec, SORT_TEST_ITEM_COUNT, 38892);

void selection_sort(mint *arr, midx n){
    for(midx i = 0;i < n;i++){
        midx minidx = i;
        for(midx j = i+1;j < n;j++){
            if(arr[j] < arr[minidx]){
                minidx = j;
            }
        }
        if(minidx != i)
            swap(&arr[i], &arr[minidx]);
    }
}

sort_test(selection_sort, SORT_TEST_ITEM_COUNT, 58931);

static void selection_sort_rec2(mint *arr, midx i, midx n){
    if(i == n)
        return;
    midx minidx = i;
    for(midx j = i + 1;j < n;j++){
        if(arr[j] < arr[minidx])
            minidx = j;
    }
    if(minidx != i)
        swap(&arr[i], &arr[minidx]);
    selection_sort_rec2(arr, i + 1, n);
}

void selection_sort_rec(mint *arr, midx n){
    selection_sort_rec2(arr, 0, n);
}

sort_test(selection_sort_rec, SORT_TEST_ITEM_COUNT, 788498);

#ifdef QUICK_SORT_VISUAL

#include <stdio.h>

static void quick_sort_print(mint *arr, midx n, midx i, midx j, midx pivot){
    printf(" ");
    for(midx k = 0; k < n;k++){
        if(k == i)
            printf(ANSI_FONT_BOLD ANSI_COLOR_BLUE);
        else if(k == j)
            printf(ANSI_FONT_BOLD ANSI_COLOR_GREEN);
        else if(k == pivot)
            printf(ANSI_FONT_BOLD ANSI_COLOR_YELLOW);
        printf("%" PRIint "  " ANSI_COLOR_RESET, arr[k]);
    }
}

#endif

static midx quick_sort_partition(mint *arr, midx m, midx n, midx total){
    mint pivot = arr[m];
    midx i = m;
    midx j = n + 1;
#ifdef QUICK_SORT_VISUAL
    dbg("Initially   : ");
    quick_sort_print(arr, total, i, j, m);
#endif
    do{
#ifdef QUICK_SORT_VISUAL
        dbg("Start itn.  : ");
        quick_sort_print(arr, total, i, j, m);
#endif
        do{
            i++;
#ifdef QUICK_SORT_VISUAL
            dbg("Increment i : ");
            quick_sort_print(arr, total, i, j, m);
#endif
        } while(i < total-1 && arr[i] < pivot);
        do{
            j--;
#ifdef QUICK_SORT_VISUAL
            dbg("Decrement j : ");
            quick_sort_print(arr, total, i, j, m);
#endif
        } while(arr[j] > pivot);
        if(i < j){
            swap(&arr[i], &arr[j]);
#ifdef QUICK_SORT_VISUAL
            dbg("Do swap i-j : ");
            quick_sort_print(arr, total, i, j, m);
#endif
        }
#ifdef QUICK_SORT_VISUAL
        printf("\n");
#endif
    } while(i < j);
    swap(&arr[m], &arr[j]);
#ifdef QUICK_SORT_VISUAL
    dbg("Do swap p-j : ");
    quick_sort_print(arr, total, i, j, m);
    printf("\n");
#endif
    return j;
}

void quick_sort2(mint *arr, midx m, midx n, midx total){
    if(m >= n)
        return;
    midx j = quick_sort_partition(arr, m, n, total);
    quick_sort2(arr, m, j - 1, total);
    quick_sort2(arr, j + 1, n, total);
}

void quick_sort(mint *arr, midx n){
#ifdef QUICK_SORT_VISUAL
    dbg("QuickSorting...");
#endif
    quick_sort2(arr, 0, n - 1, n);
#ifdef QUICK_SORT_VISUAL
    dbg("\n");
#endif
}

#ifdef QUICK_SORT_VISUAL
sort_test(quick_sort, 20, 10);
#else
sort_test(quick_sort, SORT_TEST_ITEM_COUNT, 89893);
#endif

void quick_sort_nonrec(mint *arr, midx n){
    Stack start = stack_new(n/2 + 1, 0);
    Stack stop = stack_new(n/2 + 1, 0);
    stack_push_fast(start, 0);
    stack_push_fast(stop, n - 1);
    midx total = n;
    while(!stack_is_empty(start)){
        midx m = stack_pop_fast(start);
        n = stack_pop_fast(stop);
        midx j = quick_sort_partition(arr, m, n, total);
        if(m < j - 1){
            stack_push_fast(start, m);
            stack_push_fast(stop, j - 1);
        }
        if(j + 1 < n){
            stack_push_fast(start, j + 1);
            stack_push_fast(stop, n);
        }
    }
    stack_free(start);
    stack_free(stop);
}

#ifdef QUICK_SORT_VISUAL
sort_test(quick_sort_nonrec, 20, 10);
#else
sort_test(quick_sort_nonrec, SORT_TEST_ITEM_COUNT, 87487);
#endif

static void sorted_merge(mint *arr, midx l, midx m, midx u, mint *aux){
    midx pointer = 0, mid = m + 1, low = l;
    while(low <= m && mid <= u){
        if(arr[low] > arr[mid]){
            aux[pointer] = arr[mid];
            mid++;
        }
        else{
            aux[pointer] = arr[low];
            low++;
        }
        pointer++;
    }
    while(low <= m){
        aux[pointer] = arr[low];
        low++; pointer++;
    }
    while(mid <= u){
        aux[pointer] = arr[mid];
        mid++; pointer++;
    }
    for(midx i = 0;i < pointer;i++)
        arr[i + l] = aux[i];
}

static void merge_sort2(mint *arr, midx l, midx u, mint *aux){
    if(l >= u)
        return;
    midx m = (l+u)/2;
    merge_sort2(arr, l, m, aux);
    merge_sort2(arr, m+1, u, aux);
    sorted_merge(arr, l, m, u, aux);
}

void merge_sort(mint *arr, midx n){
    mint *aux = arr_new(n);
    merge_sort2(arr, 0, n - 1, aux);
    arr_free(aux);
}

sort_test(merge_sort, SORT_TEST_ITEM_COUNT, 84982);

void merge_sort_nonrec(mint *arr, midx n){
    Stack start = stack_new(n, 0);
    Stack stop = stack_new(n, 0);
    Stack beg = stack_new(n, 0);
    Stack end = stack_new(n, 0);
    Stack mid = stack_new(n, 0);

    mint *aux = arr_new(n);

    stack_push_fast(start, 0);
    stack_push_fast(stop, n - 1);
    while(!stack_is_empty(start)){
        midx s = stack_pop_fast(start);
        midx e = stack_pop_fast(stop);
        midx m = (s + e)/2;
        stack_push_fast(beg, s); stack_push_fast(end, e); stack_push_fast(mid, m);
        if(s < m){
            stack_push_fast(start, s);
            stack_push_fast(stop, m);
        }
        if(m + 1 < e){
            stack_push_fast(start, m + 1);
            stack_push_fast(stop, e);
        }
    }
    while(!stack_is_empty(beg)){
        midx b = stack_pop_fast(beg);
        midx m = stack_pop_fast(mid);
        midx e = stack_pop_fast(end);
        sorted_merge(arr, b, m, e, aux);
    }
    arr_free(aux);
    stack_free(start); stack_free(stop);
    stack_free(beg); stack_free(mid); stack_free(end);
}

sort_test(merge_sort_nonrec, SORT_TEST_ITEM_COUNT, 986798);

static mint* heap_create(mint *arr, midx n){
    mint *heap = arr_new(n);
    for(midx i = 0;i < n;i++){
        heap[i] = arr[i];
        midx j = i;
        while(j > 0){
            midx parent = (j+1)/2 - 1;
            // Min heap
            if(heap[j] < heap[parent]){
                swap(&heap[j], &heap[parent]);
                j = parent;
                continue;
            }
            break;
        }
    }
    return heap;
}

static void heap_rebuild(mint *arr, midx n){
    if(n == 1)
        return;
    midx j = 0;
    while(1){
        midx leftChild = (j * 2) + 1, rightChild = (j * 2) + 2;
        midx nextCheck = j;
        mint *min = &arr[j];
        if(leftChild < n && arr[leftChild] < *min){
            min = &arr[leftChild];
            nextCheck = leftChild;
        }
        if(rightChild < n && arr[rightChild] < *min){
            min = &arr[rightChild];
            nextCheck = rightChild;
        }
        if(*min != arr[j]){
            swap(min, &arr[j]);
            j = nextCheck;
            continue;
        }
        break;
    }
}

void heap_sort(mint *arr, midx n){
    tst_pause("Creating heap");
    mint *heap = heap_create(arr, n);
    tst_resume(NULL);
    for(midx i = n, j = 0;i > 0;i--,j++){
        arr[j] = heap[0];
        heap[0] = heap[i - 1];
        heap_rebuild(heap, i - 1);
    }
    arr[n - 1] = heap[0];
    arr_free(heap);
}

sort_test(heap_sort, SORT_TEST_ITEM_COUNT, 985893);

void test_sort(){
    TEST("Bubble Sort", test_bubble_sort());
    TEST("Bubble Sort Recursive", test_bubble_sort_rec());
    TEST("Bubble Sort Advanced", test_bubble_sort_adv());
    TEST("Insertion Sort", test_insertion_sort());
    TEST("Insertion Sort Recursive", test_insertion_sort_rec());
    TEST("Selection Sort", test_selection_sort());
    TEST("Selection Sort Recursive", test_selection_sort_rec());
    TEST("Quick Sort", test_quick_sort());
    TEST("Quick Sort Nonrecursive", test_quick_sort_nonrec());
    TEST("Merge Sort", test_merge_sort());
    TEST("Merge Sort Nonrecursive", test_merge_sort_nonrec());
    TEST("Heap Sort", test_heap_sort());
}
