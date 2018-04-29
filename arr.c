#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arr.h"
#include "display.h"
#include "test.h"

void arr_fill_int(mint *arr, midx count, ...){
    va_list list;
    va_start(list, count);
    for(midx i = 0;i < count;i++){ 
        // Default int size is 32, which is also
        // default integer argument size. Casting it to 
        // mint aka int64_t introduces some nasty
        // size mismatch bug at the multiple of 5,
        // producing garbage values as a result of
        // casting
        //
        // One remedy should be explicitly casting each
        // argument to mint before/while passing.
        // The other is passing only integer elements,
        // while the cast will only be `int`.
        //
        // Hence, two separate methods are given to
        // justify these causes.
        mint el = va_arg(list, int);

        arr[i] = el;
    }
    va_end(list);
}

void arr_fill_rand(mint *arr, midx count, mint range, SampleCase scase){
    srand(time(NULL));
    mint start = random() % range;
    while((scase == SAMPLE_CASE_BEST && MINT_MAX - start < count)
            || (scase == SAMPLE_CASE_WORST && MINT_MAX + start < count))
        start = random() % range;
    
    for(midx i = 0;i < count;i++){
        arr[i] = scase == SAMPLE_CASE_AVERAGE ? random() % range 
                : scase == SAMPLE_CASE_BEST ? start++
                : start--;
    }
}

void arr_fill(mint *arr, midx count, ...){
    va_list list;
    va_start(list, count);
    for(midx i = 0;i < count;i++){ 
        mint el = va_arg(list, mint);
        arr[i] = el;
    }
    va_end(list);
}

mint* arr_create(){
reinput:
    pgrn(ANSI_COLOR_GREEN ANSI_FONT_BOLD "[Input] " ANSI_COLOR_RESET);
    info("Size of the array : ");
    midx size;
    scanf("%" SCNidx, &size);
    if(size < 1){
        err("Size must be >= 1!");
        goto reinput;
    }
    mint* arr = arr_new(size);
    if(!arr){
        err("Array can not be initialized!");
        exit(1);
    }
    for(midx i = 0;i < size;i++){
        pblue(ANSI_FONT_BOLD ANSI_COLOR_BLUE "[Element %" PRIidx "] " ANSI_COLOR_RESET, i+1);
        scanf("%" SCNint, &arr[i]);
    }
    return arr;
}

static mint test_arr_fill(mint *arr){
    arr_fill_int(arr, 2, 2, 3);
    if(arr[1] != 3){
        return 0;
    }
    arr_fill_int(arr, 5, 8, 2, 34, 2, 1);
    if(arr[4] != 1){
        return 0;
    }
    if(arr[0] != 8){
        return 0;
    }
    return 1;
}

void test_arr(){
    mint *arr = arr_new(10);
    TEST("Array Create", arr);
    TEST("Array Fill", test_arr_fill(arr));
    arr_free(arr);
}
