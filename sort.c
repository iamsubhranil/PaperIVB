#include "sort.h"
#include "arr.h"
#include "test.h"
#include "display.h"
#include "stack.h"
#include "queue.h"

#ifdef SORT_ENABLE_VISUAL

#include "histogram.h"

#endif

mint check_sort(mint *arr, midx n, SortType type){
    if(type == SORT_TYPE_ASCENDING){
        for(midx i = 0;i < n-1;i++){
            if(arr[i] > arr[i+1]){
                dbg("arr[%" PRIint "] : %" PRIint "\t arr[%" PRIint "] : %" PRIint "\n", i, arr[i], i+1, arr[i+1]);
                return 0;
            }
        }
    }
    else{
        for(midx i = 0;i < n-1;i++){
            if(arr[i] < arr[i+1]){
                dbg("arr[%" PRIint "] : %" PRIint "\t arr[%" PRIint "] : %" PRIint "\n", i, arr[i], i+1, arr[i+1]);
                return 0;
            }
        }
    
    }
    return 1;
}

#define sort_test2(name, sz, customrange) \
    mint test_##name(){ \
        mint size = sz; \
        tst_pause("Creating array"); \
        mint *arr = arr_new(size); \
        tst_pause("Generating random input : Best Case"); \
        arr_fill_rand(arr, size, size, SAMPLE_CASE_BEST); \
        tst_resume("Best Case"); \
        name(arr, size); \
        tst_pause("Checking result"); \
        if(!check_sort(arr, size, SORT_TYPE_ASCENDING)){ \
            goto _test_fail_##name; \
        } \
        tst_pause("Generating random input : Worst Case"); \
        arr_fill_rand(arr, size, -size, SAMPLE_CASE_WORST); \
        tst_resume("Worst Case"); \
        name(arr, size); \
        tst_pause("Checking result"); \
        if(!check_sort(arr, size, SORT_TYPE_ASCENDING)){ \
            goto _test_fail_##name; \
        } \
        tst_pause("Generating random input : Average Case"); \
        arr_fill_rand(arr, size, customrange, SAMPLE_CASE_AVERAGE); \
        tst_resume("Average Case"); \
        name(arr, size); \
        tst_pause("Checking result"); \
        if(!check_sort(arr, size, SORT_TYPE_ASCENDING)) { \
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

#ifdef SORT_ENABLE_VISUAL
#define sort_test(name, sz, customrange) sort_test2(name, 20, 100)
#else
#define sort_test(name, sz, customrange) sort_test2(name, sz, customrange)
#endif

static void swap(mint *p1, mint *p2){
    mint p3 = *p1;
    *p1 = *p2;
    *p2 = p3;
}

void bubble_sort(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Bubble Sort");
#endif
    for(midx pass = 0;pass < n;pass++){
        for(midx i = 0;i < n-pass-1;i++){
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 2, pass, ANSI_COLOR_BLUE, i, ANSI_COLOR_GREEN);
#endif
            if(arr[i] > arr[i+1]){
                swap(&arr[i], &arr[i+1]);
            }
        }
    }
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 0);
#endif
}

#ifdef SORT_ENABLE_TEST_BUBBLE
sort_test(bubble_sort, SORT_TEST_ITEM_COUNT, 372);
#endif

static void bubble_sort_rec2(mint *arr, midx pass, midx n){
    if(pass == n)
        return;
    for(midx i = 0;i < n-pass-1;i++){
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 2, pass, ANSI_COLOR_BLUE, i, ANSI_COLOR_GREEN);
#endif
        if(arr[i] > arr[i+1]){
            swap(&arr[i], &arr[i+1]);
        }
    }
    bubble_sort_rec2(arr, pass+1, n);
}

void bubble_sort_rec(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Bubble Sort Recursive");
#endif
    bubble_sort_rec2(arr, 0, n);
}

#ifdef SORT_ENABLE_TEST_BUBBLE_REC
sort_test(bubble_sort_rec, SORT_TEST_ITEM_COUNT, 483);
#endif

void bubble_sort_adv(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Bubble Sort Advanced");
#endif
    for(midx pass = 0;pass < n;pass++){
        mint flag = 0;
        for(midx i = 0;i < n - pass - 1;i++){
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 2, pass, ANSI_COLOR_BLUE, i, ANSI_COLOR_GREEN);
#endif
            if(arr[i] > arr[i+1]){
                swap(&arr[i], &arr[i+1]);
                flag = 1;
            }
        }
        if(!flag)
            return;
    }
}

#ifdef SORT_ENABLE_TEST_BUBBLE_ADV
sort_test(bubble_sort_adv, SORT_TEST_ITEM_COUNT, 281);
#endif

void insertion_sort(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Insertion Sort");
#endif
    for(midx i = 0; i < n; i++){
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 1, i, ANSI_COLOR_GREEN);
#endif
        mint k = arr[i];
        midx j = i - 1;
        while(j >= 0 && k < arr[j]){
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 2, i, ANSI_COLOR_GREEN, j, ANSI_COLOR_BLUE, k, ANSI_COLOR_RED);
#endif
            arr[j+1] = arr[j];
            j--;
        }
        arr[j + 1] = k;
    }
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 0);
#endif
}

#ifdef SORT_ENABLE_TEST_INSERTION
sort_test(insertion_sort, SORT_TEST_ITEM_COUNT, 3478387);
#endif

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

#ifdef SORT_ENABLE_TEST_INSERTION_REC
sort_test(insertion_sort_rec, SORT_TEST_ITEM_COUNT, 38892);
#endif

void selection_sort(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Selection Sort");
#endif
    for(midx i = 0;i < n;i++){
        midx minidx = i;
        for(midx j = i+1;j < n;j++){
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 2, i, ANSI_COLOR_GREEN, j, ANSI_COLOR_BLUE);
#endif
            if(arr[j] < arr[minidx]){
                minidx = j;
            }
        }
        if(minidx != i)
            swap(&arr[i], &arr[minidx]);
    }
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 0);
#endif
}

#ifdef SORT_ENABLE_TEST_SELECTION
sort_test(selection_sort, SORT_TEST_ITEM_COUNT, 58931);
#endif

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

#ifdef SORT_ENABLE_TEST_SELECTION_REC
sort_test(selection_sort_rec, SORT_TEST_ITEM_COUNT, 788498);
#endif

static midx quick_sort_partition(mint *arr, midx m, midx n, midx total){
    mint pivot = arr[m];
    midx i = m;
    midx j = n + 1;
    do{
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
        do{
            i++;
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
        } while(i < total-1 && arr[i] < pivot);
        do{
            j--;
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
        } while(arr[j] > pivot);
        if(i < j){
            swap(&arr[i], &arr[j]);
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
#endif
        }
    } while(i < j);
    swap(&arr[m], &arr[j]);
#ifdef SORT_ENABLE_VISUAL
    histo_draw(arr, total, 3, i, ANSI_COLOR_BLUE, j, ANSI_COLOR_GREEN, m, ANSI_COLOR_RED);
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
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Quick Sort");
#endif
    quick_sort2(arr, 0, n - 1, n);
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 0);
#endif
}

#ifdef SORT_ENABLE_TEST_QUICK
sort_test(quick_sort, SORT_TEST_ITEM_COUNT, 89893);
#endif

void quick_sort_nonrec(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Quick Sort Nonrecursive");
#endif
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

#ifdef SORT_ENABLE_TEST_QUICK_NONREC
sort_test(quick_sort_nonrec, SORT_TEST_ITEM_COUNT, 87487);
#endif

#ifdef SORT_ENABLE_VISUAL
static void sorted_merge(mint *arr, midx l, midx m, midx u, mint *aux, mint total){
#else
static void sorted_merge(mint *arr, midx l, midx m, midx u, mint *aux){
#endif
    midx pointer = 0, mid = m + 1, low = l;
    while(low <= m && mid <= u){
#ifdef SORT_ENABLE_VISUAL
        histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, m, ANSI_COLOR_RED, mid, ANSI_COLOR_GREEN);
#endif
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
#ifdef SORT_ENABLE_VISUAL
        histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, m, ANSI_COLOR_RED, mid, ANSI_COLOR_GREEN);
#endif
        aux[pointer] = arr[low];
        low++; pointer++;
    }
    while(mid <= u){
#ifdef SORT_ENABLE_VISUAL
        histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, m, ANSI_COLOR_RED, mid, ANSI_COLOR_GREEN);
#endif
        aux[pointer] = arr[mid];
        mid++; pointer++;
    }
    for(midx i = 0;i < pointer;i++){
#ifdef SORT_ENABLE_VISUAL
        histo_draw(arr, total, 3, low, ANSI_COLOR_BLUE, i+l, ANSI_COLOR_RED, mid, ANSI_COLOR_GREEN);
#endif
        arr[i + l] = aux[i];
    }
}

#ifdef SORT_ENABLE_VISUAL
static void merge_sort2(mint *arr, midx l, midx u, mint *aux, mint total){
#else
static void merge_sort2(mint *arr, midx l, midx u, mint *aux){
#endif
    if(l >= u)
        return;
    midx m = (l+u)/2;
#ifdef SORT_ENABLE_VISUAL
    merge_sort2(arr, l, m, aux, total);
    merge_sort2(arr, m+1, u, aux, total);
    sorted_merge(arr, l, m, u, aux, total);
#else
    merge_sort2(arr, l, m, aux);
    merge_sort2(arr, m+1, u, aux);
    sorted_merge(arr, l, m, u, aux);
#endif
}

void merge_sort(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Merge Sort");
#endif
    mint *aux = arr_new(n);
#ifdef SORT_ENABLE_VISUAL
    merge_sort2(arr, 0, n - 1, aux, n);
    histo_draw(arr, n, 0);
#else
    merge_sort2(arr, 0, n - 1, aux);
#endif
    arr_free(aux);
}

#ifdef SORT_ENABLE_TEST_MERGE
sort_test(merge_sort, SORT_TEST_ITEM_COUNT, 986798);
#endif

void merge_sort_nonrec(mint *arr, midx n){
    Stack start = stack_new(n, 0);
    Stack stop = stack_new(n, 0);
    Stack beg = stack_new(n, 0);
    Stack end = stack_new(n, 0);
    Stack mid = stack_new(n, 0);

    mint *aux = arr_new(n);

    stack_push_fast(start, 0);
    stack_push_fast(stop, n - 1);
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Merge Sort Nonrecursive");
#endif
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
#ifdef SORT_ENABLE_VISUAL
        sorted_merge(arr, b, m, e, aux, n);
#else
        sorted_merge(arr, b, m, e, aux);
#endif
    }

    arr_free(aux);
    stack_free(start); stack_free(stop);
    stack_free(beg); stack_free(mid); stack_free(end);
}

#ifdef SORT_ENABLE_TEST_MERGE_NONREC
sort_test(merge_sort_nonrec, SORT_TEST_ITEM_COUNT, 986798);
#endif

static mint* heap_create(mint *arr, midx n){
    mint *heap = arr_new(n);
    for(midx i = 0;i < n;i++){
        heap[i] = arr[i];
        midx j = i;
        while(j > 0){
            midx parent = (j+1)/2 - 1;
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, n, 2, j, ANSI_COLOR_BLUE, parent, ANSI_COLOR_RED);
#endif
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

#ifdef SORT_ENABLE_VISUAL
static void heap_rebuild(mint *arr, midx n, midx total){
#else
static void heap_rebuild(mint *arr, midx n){
#endif
    if(n == 1)
        return;
    midx j = 0;
    while(1){
        midx leftChild = (j * 2) + 1, rightChild = (j * 2) + 2;
        midx min = j;
#ifdef SORT_ENABLE_VISUAL
            histo_draw(arr, total, 3, leftChild, ANSI_COLOR_BLUE, j, ANSI_COLOR_RED, rightChild, ANSI_COLOR_GREEN);
#endif
        if(leftChild < n && arr[leftChild] < arr[min]){
            min = leftChild;
        }
        if(rightChild < n && arr[rightChild] < arr[min]){
            min = rightChild;
        }
        if(min != j){
            swap(&arr[min], &arr[j]);
            j = min;
            continue;
        }
        break;
    }
}

void heap_sort(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Heap Sort");
#endif
    mint *heap = heap_create(arr, n);
    for(midx i = n, j = 0;i > 0;i--,j++){
        arr[j] = heap[0];
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

#ifdef SORT_ENABLE_TEST_HEAP
sort_test(heap_sort, SORT_TEST_ITEM_COUNT, 985893);
#endif

void radix_sort(mint *arr, midx n){
#ifdef SORT_ENABLE_VISUAL
    histo_set_title("Radix Sort");
#endif
    mint max = arr[0];
    for(midx i = 1;i < n;i++)
        if(arr[i] > max)
            max =  arr[i];

    mint passes = 1;
    if(max < 0)
        max = -max;
    while(max > 0){
        passes++;
        max /= 10;
    }

    Queue queue[20];
    for(mint i = 0;i < 20;i++)
        queue[i] = queue_new(n);

    mint pwr = 1;
    for(mint i = 0;i < passes;i++){
        for(mint j = 0;j < n;j++){
            mint tmp = arr[j] < 0 ? -arr[j] : arr[j];
            mint bucket = (tmp / pwr) % 10;
            if(arr[j] >= 0)
                bucket += 10;
            else{
                bucket = 9 - bucket;
            }
            queue_insert(queue[bucket], arr[j]);
        }
        
        mint bucket = 0;
        
        for(mint j = 0;j < n;){
            while(queue_is_empty(queue[bucket]))
                bucket++;
            while(!queue_is_empty(queue[bucket])){
#ifdef SORT_ENABLE_VISUAL
                histo_draw(arr, n, 1, j, ANSI_COLOR_BLUE);
#endif
                arr[j] = queue_delete(queue[bucket]);
                j++;
            }
            bucket++;
        }
        pwr *= 10;
    }

    for(mint i = 0;i < 20;i++)
        queue_free(queue[i]);

#ifdef SORT_ENABLE_VISUAL
    histo_draw(arr, n, 0);
#endif
}

#ifdef SORT_ENABLE_TEST_RADIX
sort_test(radix_sort, SORT_TEST_ITEM_COUNT, 58683);
#endif

void test_sort(){
#ifdef SORT_ENABLE_TEST_BUBBLE
    TEST("Bubble Sort", test_bubble_sort());
#endif
#ifdef SORT_ENABLE_TEST_BUBBLE_REC
    TEST("Bubble Sort Recursive", test_bubble_sort_rec());
#endif
#ifdef SORT_ENABLE_TEST_BUBBLE_ADV
    TEST("Bubble Sort Advanced", test_bubble_sort_adv());
#endif
#ifdef SORT_ENABLE_TEST_INSERTION
    TEST("Insertion Sort", test_insertion_sort());
#endif
#ifdef SORT_ENABLE_TEST_INSERTION_REC
    TEST("Insertion Sort Recursive", test_insertion_sort_rec());
#endif
#ifdef SORT_ENABLE_TEST_SELECTION
    TEST("Selection Sort", test_selection_sort());
#endif
#ifdef SORT_ENABLE_TEST_SELECTION_REC
    TEST("Selection Sort Recursive", test_selection_sort_rec());
#endif
#ifdef SORT_ENABLE_TEST_QUICK
    TEST("Quick Sort", test_quick_sort());
#endif
#ifdef SORT_ENABLE_TEST_QUICK_NONREC
    TEST("Quick Sort Nonrecursive", test_quick_sort_nonrec());
#endif
#ifdef SORT_ENABLE_TEST_MERGE
    TEST("Merge Sort", test_merge_sort());
#endif
#ifdef SORT_ENABLE_TEST_MERGE_NONREC
    TEST("Merge Sort Nonrecursive", test_merge_sort_nonrec());
#endif
#ifdef SORT_ENABLE_TEST_HEAP
    TEST("Heap Sort", test_heap_sort());
#endif
#ifdef SORT_ENABLE_TEST_RADIX
    TEST("Radix Sort", test_radix_sort());
#endif
}
