#include <time.h>
#include <stdio.h>
#include <string.h>

#include "test.h"
#include "display.h"

static clock_t start = 0, end = 0, elapsed = 0;

static u8 header_shown = 0;

static int test_header_pad = (TEST_NAME_WIDTH / 2) - 4, total_width = TEST_NAME_WIDTH + 11;

static void header_underline(int linesize){
    while(linesize--)
        printf("=");
}

static void header(){
    printf(ANSI_FONT_BOLD " Count \t");
    printf("%*s%s%*s", test_header_pad, " ", "TestName", (TEST_NAME_WIDTH - test_header_pad - 4), " ");
    printf("Time\n");
    header_underline(7);
    printf(" ");
    header_underline(TEST_NAME_WIDTH);
    printf(" ");
    header_underline(10);
    printf(ANSI_COLOR_RESET);
    header_shown = 1;
}

static void line_clear(){
    printf("\r");
    for(siz i = 0;i < 110;i++)
        printf(" ");
}

static const char *testName[] = {"SampleTest"};
static i64 paused = 0;
static u64 test_count = 0, done_tests = 1, global_done_tests = 1;

static void print_test_count(){
    if(test_count >= done_tests){
        pcyn("\r%3" Pu64 "/%-3" Pu64 " ", done_tests, test_count);
    }
    else
        pylw(ANSI_FONT_BOLD "\r%6" Pu64 "  ", global_done_tests);
}

static void test_count_incr(){
    if(test_count >= done_tests)
        done_tests++;
    else
        global_done_tests++;
}

void tst_suite_start(const char *name, u64 count){
    if(!paused && start == 0)
        header();
    test_count = 0;
    pblue("\n");
    print_test_count();
    test_count = count;

    siz s = strlen(name);
    int left = (total_width / 2) - (s / 2) - 1;
    if(left > total_width)
        left = 0;
    
    for(int j = 0;j < left;j++)
        printf(ANSI_FONT_BOLD "=");
    
    pcyn(ANSI_FONT_BOLD " %s ", name);
    
    for(int j = left + s + 2;j < total_width;j++)
        printf(ANSI_FONT_BOLD "=");
    
    printf(ANSI_COLOR_RESET);
}

void tst_suite_end(){
    test_count = 0;
    done_tests = 1;
    int s = 0;
    pblue("\n");
    print_test_count();
    while(s++ < total_width)
        printf(ANSI_FONT_BOLD "=");
    test_count_incr();
}

void tst_start(const char *name){
    if(!header_shown)
        header();
    pblue("\n");
    print_test_count();
    paused = 0;
    pylw(ANSI_FONT_BOLD "\u25d5 ");
    printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET, TEST_NAME_WIDTH, name);
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
    print_test_count();
    pgrn( "\u2714 " ANSI_COLOR_RESET);
    printf( "%-*s" ANSI_COLOR_RESET "%2.6fs", TEST_NAME_WIDTH, testName[0], time_diff());
    fflush(stdout);
    test_count_incr();
    elapsed = 0;
    start = end = 1;
}

void tst_fail(){
    if(!paused)
        end = clock();
    line_clear();
    print_test_count();
    pred( ANSI_FONT_BOLD "\u2718 " ANSI_COLOR_RESET);
    printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET "%2.6fs", TEST_NAME_WIDTH, testName[0], time_diff());
    fflush(stdout);
    test_count_incr();
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
    print_test_count();
    pylw(ANSI_FONT_BOLD "\u27d1 " ANSI_COLOR_RESET);
    if(reason == NULL)
        printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET, TEST_NAME_WIDTH, testName[0]);
    else
        printf( ANSI_FONT_BOLD "%-*s: %s" ANSI_COLOR_RESET, TEST_NAME_WIDTH, testName[0], reason);
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
    print_test_count();
    pylw( ANSI_FONT_BOLD "\u25d5 " ANSI_COLOR_RESET);
    if(status)
        printf(ANSI_FONT_BOLD "%-*s : %s" ANSI_COLOR_RESET, TEST_NAME_WIDTH, testName[0], status);
    else
        printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET, TEST_NAME_WIDTH, testName[0]);
    fflush(stdout);
    start = clock(); 
}
