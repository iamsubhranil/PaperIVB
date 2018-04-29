#include <time.h>
#include <stdio.h>
#include <string.h>

#include "test.h"
#include "display.h"
#include "common.h"

static clock_t start, end;

void tst_start(const char *name){
    pylw( ANSI_FONT_BOLD "\n[Test] " ANSI_COLOR_RESET);
    printf("Running " ANSI_FONT_BOLD "%s" ANSI_COLOR_RESET, name);
    fflush(stdout);
    start = clock();
}

static double time_diff(){
    return (double)(end - start)/CLOCKS_PER_SEC;
}

void tst_pass(const char *name){
    end = clock();
    pgrn( ANSI_FONT_BOLD "\r[Passed] " ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET " : %fs\t", name, time_diff());
    fflush(stdout);
}

void tst_fail(const char *name){
    end = clock();
    pred( ANSI_FONT_BOLD "\r[Failed] " ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET " : %fs\t", name, time_diff());
    fflush(stdout);
}
