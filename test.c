#include <stdio.h>
#include <string.h>
#include <time.h>

#include "display.h"
#include "test.h"

static clock_t start = 0, end = 0, elapsed = 0;
double         suite_total_time = 0;

static u8 header_shown = 0;

static int test_header_pad = (TEST_NAME_WIDTH / 2) - 4,
           total_width     = TEST_NAME_WIDTH + 11;

static const char *str_pass = "\u2714", *str_fail = "\u2718",
                  *str_clock = "\u25d5";

static void header_underline(int linesize) {
	while(linesize--) printf("=");
}

static void header() {
	printf(ANSI_FONT_BOLD " Count \t");
	printf("%*s%s%*s", test_header_pad, " ", "TestName",
	       (TEST_NAME_WIDTH - test_header_pad - 4), " ");
	printf("Time\n");
	header_underline(7);
	printf(" ");
	header_underline(TEST_NAME_WIDTH);
	printf(" ");
	header_underline(10);
	printf(ANSI_COLOR_RESET);
	header_shown = 1;
}

static void line_clear() {
	printf("\r");
	for(siz i = 0; i < 110; i++) printf(" ");
}

static const char *testName[]       = {"SampleTest"};
static i64         paused           = 0;
static u64         suite_test_count = 0, suite_tests_done = 1,
           global_suite_tests_done = 1;
static u64 suite_tests_passed = 0, suite_tests_failed = 0;

static void print_suite_test_count() {
	if(suite_test_count >= suite_tests_done) {
		pblue("\r%3" Pu64 "/%-3" Pu64 " ", suite_tests_done, suite_test_count);
	} else
		pylw(ANSI_FONT_BOLD "\r%6" Pu64 "  ", global_suite_tests_done);
}

static void suite_test_count_incr(u8 passed) {
	if(suite_test_count >= suite_tests_done) {
		suite_tests_done++;
		if(passed)
			suite_tests_passed++;
		else
			suite_tests_failed++;
	} else
		global_suite_tests_done++;
}

void tst_suite_start(const char *name, u64 count) {
	if(!paused && start == 0)
		header();
	suite_test_count = 0;
	pblue("\n");
	print_suite_test_count();
	suite_test_count = count;

	siz s    = strlen(name);
	int left = (total_width / 2) - (s / 2) - 1;
	if(left > total_width)
		left = 0;

	for(int j = 0; j < left; j++) printf(ANSI_FONT_BOLD "=");

	pcyn(ANSI_FONT_BOLD " %s ", name);

	for(int j = left + s + 2; j < total_width; j++) printf(ANSI_FONT_BOLD "=");

	printf(ANSI_COLOR_RESET);
}

void tst_suite_end() {
	u64 bak          = suite_test_count;
	suite_test_count = 0;
	suite_tests_done = 1;
	int gap          = total_width < 30 ? 1 : (total_width - 31) / 2;
	pblue("\n");
	print_suite_test_count();
	printf(ANSI_FONT_BOLD "= ");
	pgrn(ANSI_FONT_BOLD "%s %2" Pu64 "/%-2" Pu64 " ", str_pass,
	     suite_tests_passed, bak);
	for(int i = 0; i < gap; i++) printf(ANSI_FONT_BOLD "=");
	pred(ANSI_FONT_BOLD " %s %2" Pu64 "/%-2" Pu64 " ", str_fail,
	     suite_tests_failed, bak);
	for(int i = 0; i < gap; i++) printf(ANSI_FONT_BOLD "=");
	pylw(ANSI_FONT_BOLD " %s ", str_clock);
	printf(ANSI_FONT_BOLD "%3.6f =", suite_total_time);
	suite_test_count_incr(0);
	suite_tests_failed = suite_tests_passed = suite_total_time = 0;
}

void tst_start(const char *name) {
	if(!header_shown)
		header();
	pblue("\n");
	print_suite_test_count();
	paused = 0;
	pylw(ANSI_FONT_BOLD "\u25d5 ");
	printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET, TEST_NAME_WIDTH, name);
	fflush(stdout);
	testName[0] = name;
	start       = clock();
}

static double time_diff() {
	return (double)(end - start + elapsed) / CLOCKS_PER_SEC;
}

void tst_pass() {
	if(!paused)
		end = clock();
	line_clear();
	print_suite_test_count();
	pgrn("\u2714 " ANSI_COLOR_RESET);
	printf("%-*s" ANSI_COLOR_RESET "%2.6fs", TEST_NAME_WIDTH, testName[0],
	       time_diff());
	fflush(stdout);
	if(suite_test_count >= suite_tests_done)
		suite_total_time += time_diff();
	suite_test_count_incr(1);
	elapsed = 0;
	start = end = 1;
}

void tst_fail() {
	if(!paused)
		end = clock();
	line_clear();
	print_suite_test_count();
	pred(ANSI_FONT_BOLD "\u2718 " ANSI_COLOR_RESET);
	printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET "%2.6fs", TEST_NAME_WIDTH,
	       testName[0], time_diff());
	fflush(stdout);
	if(suite_test_count >= suite_tests_done)
		suite_total_time += time_diff();
	suite_test_count_incr(0);
	elapsed = 0;
	start = end = 1;
}

void tst_pause(const char *reason) {
	if(!paused) {
		end = clock();
		elapsed += (end - start);
		end = start = 0;
		paused      = 1;
	}
	line_clear();
	print_suite_test_count();
	pylw(ANSI_FONT_BOLD "\u27a1 " ANSI_COLOR_RESET);
	if(reason == NULL)
		printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET, TEST_NAME_WIDTH,
		       testName[0]);
	else
		printf(ANSI_FONT_BOLD "%-*s: %s" ANSI_COLOR_RESET, TEST_NAME_WIDTH,
		       testName[0], reason);
	fflush(stdout);
}

void tst_resume(const char *status) {
	if(!paused) { // Do not incur the display latency overhead
		          // if the method was called to update
		          // the status of a running task
		end = clock();
		elapsed += (end - start);
		end = start = 1;
	} else
		paused = 0;
	line_clear();
	print_suite_test_count();
	pylw(ANSI_FONT_BOLD "\u25d5 " ANSI_COLOR_RESET);
	if(status)
		printf(ANSI_FONT_BOLD "%-*s : %s" ANSI_COLOR_RESET, TEST_NAME_WIDTH,
		       testName[0], status);
	else
		printf(ANSI_FONT_BOLD "%-*s" ANSI_COLOR_RESET, TEST_NAME_WIDTH,
		       testName[0]);
	fflush(stdout);
	start = clock();
}
