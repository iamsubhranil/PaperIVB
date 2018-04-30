#include <time.h>
#include <stdio.h>
#include <string.h>

#include "test.h"
#include "display.h"
#include "common.h"

static clock_t start = 0, end = 0, elapsed = 0;

static void header(){
    printf(ANSI_FONT_BOLD " Status              TestName               Time" ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "\n======== =============================== ==========" ANSI_COLOR_RESET);
}

static void line_clear(){
    printf("\r");
    for(midx i = 0;i < 80;i++)
        printf(" ");
}

static const char *testName[] = {"SampleTest"};
static mint paused = 0;

void tst_start(const char *name){
    if(!paused && start == 0)
        header();
    paused = 0;
    pylw( ANSI_FONT_BOLD "\n[Running] " ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET, name);
    fflush(stdout);
    testName[0] = name;
    start = clock();
}

static double time_diff(){
    return (double)(end - start + elapsed)/CLOCKS_PER_SEC;
}

void tst_pass(){
    if(!paused)
        end = clock();
    line_clear();
    pgrn( ANSI_FONT_BOLD "\r[Passed] " ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET "   %fs", testName[0], time_diff());
    fflush(stdout);
    elapsed = 0;
    start = end = 1;
}

void tst_fail(){
    if(!paused)
        end = clock();
    line_clear();
    pred( ANSI_FONT_BOLD "\r[Failed] " ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET "   %fs", testName[0], time_diff());
    fflush(stdout);
    elapsed = 0;
    start = end = 1;
}

void tst_pause(const char *reason){
    if(!paused){
        end = clock();
        elapsed += (end - start);
        end = start = 0;
        paused = 1;
    }
    line_clear();
    pylw(ANSI_FONT_BOLD "\r[Paused] " ANSI_COLOR_RESET);
    if(reason == NULL)
        printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET, testName[0]);
    else
        printf( ANSI_FONT_BOLD "%-30s : %s" ANSI_COLOR_RESET, testName[0], reason);
    fflush(stdout);
}

void tst_resume(const char *status){
    if(!paused){ // Do not incur the display latency overhead
                // if the method was called to update
                // the status of a running task
        end = clock();
        elapsed += (end - start);
        end = start = 1;
    }
    else
        paused = 0;
    line_clear();
    pylw( ANSI_FONT_BOLD "\r[Running] " ANSI_COLOR_RESET);
    if(status)
        printf(ANSI_FONT_BOLD "%-30s : %s" ANSI_COLOR_RESET, testName[0], status);
    else
        printf(ANSI_FONT_BOLD "%-30s" ANSI_COLOR_RESET, testName[0]);
    fflush(stdout);
    start = clock(); 
}
