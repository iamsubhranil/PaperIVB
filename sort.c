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
        mint *arr = arr_new(size); \
        arr_fill_rand(arr, size, size, SAMPLE_CASE_BEST); \
        name(arr, size); \
        if(!check_sort(arr, size)){ \
            goto _test_fail_##name; \
        } \
        arr_fill_rand(arr, size, -size, SAMPLE_CASE_WORST); \
        name(arr, size); \
        if(!check_sort(arr, size)){ \
            goto _test_fail_##name; \
        } \
        arr_fill_rand(arr, size, customrange, SAMPLE_CASE_AVERAGE); \
        name(arr, size); \
        if(!check_sort(arr, size)) { \
            goto _test_fail_##name; \
        } \
        goto _test_done_##name; \
        _test_fail_##name: \
        free(arr); \
        return 0; \
        _test_done_##name: \
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

sort_test(bubble_sort, 10000, 372);

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

sort_test(bubble_sort_rec, 10000, 483);

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

sort_test(bubble_sort_adv, 10000, 281);

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

sort_test(insertion_sort, 10000, 3478387);

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

sort_test(insertion_sort_rec, 10000, 38892);

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

sort_test(selection_sort, 10000, 58931);

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

sort_test(selection_sort_rec, 10000, 788498);

#include <stdio.h>

static midx quick_sort_partition(mint *arr, midx m, midx n, midx total){
    mint pivot = arr[m];
    midx i = m;
    midx j = n + 1;
    //dbg(" i : %" PRIidx " j : %" PRIidx, i, j);
    //fflush(stdout);
    do{
        //dbg("Checking i");
        //fflush(stdout);
        do{
            i++;
            //dbg(" i : %" PRIidx, i);
            //fflush(stdout);
        } while(i < total-1 && arr[i] < pivot);
        //dbg("Checking j");
        do{
            j--;
            //dbg(" j : %" PRIidx, j);
            //fflush(stdout);
        } while(arr[j] > pivot);
        if(i < j)
            swap(&arr[i], &arr[j]);
    } while(i < j);
    swap(&arr[m], &arr[j]);
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
    quick_sort2(arr, 0, n - 1, n);
}

sort_test(quick_sort, 10000, 8958);

void quick_sort_nonrec(mint *arr, midx n){
    Stack start = stack_new(n/2 + 1);
    Stack stop = stack_new(n/2 + 1);
    stack_push(start, 0);
    stack_push(stop, n - 1);
    midx total = n;
    while(!stack_empty(start)){
        midx m = stack_pop(start);
        n = stack_pop(stop);
        midx j = quick_sort_partition(arr, m, n, total);
        if(m < j - 1){
            stack_push(start, m);
            stack_push(stop, j - 1);
        }
        if(j + 1 < n){
            stack_push(start, j + 1);
            stack_push(stop, n);
        }
    }
    stack_free(start);
    stack_free(stop);
}

sort_test(quick_sort_nonrec, 10000, 56885);

void test_sort(){
    //*
    TEST("Bubble Sort", test_bubble_sort());
    TEST("Bubble Sort Recursive", test_bubble_sort_rec());
    TEST("Bubble Sort Advanced", test_bubble_sort_adv());
    TEST("Insertion Sort", test_insertion_sort());
    TEST("Insertion Sort Recursive", test_insertion_sort_rec());
    TEST("Selection Sort", test_selection_sort());
    TEST("Selection Sort Recursive", test_selection_sort_rec());
    //*/
    TEST("Quick Sort", test_quick_sort());
    TEST("Quick Sort Nonrecursive", test_quick_sort_nonrec());
}
