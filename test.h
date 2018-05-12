#pragma once

#include <stdarg.h>

#include "common.h"

#ifndef TEST_NAME_WIDTH
#define TEST_NAME_WIDTH 25
#endif

void tst_pass();
void tst_fail();
void tst_start(const char *name);

// Start a test suite with the given name and count number of tests
// It will display a suite header like
// ===== MyTests =======================
// and the tests will show a count of number of tests done over
// number of total tests. If count is = 0, no counter will be shown.
void tst_suite_start(const char *name, u64 count);

// End a previously started test suite
void tst_suite_end();

// Pause the presently running test. This will
// also pause the associated timers. The test
// can be ended using either of tst_pass or tst_fail
// calls, or can be resumed using tst_resume from
// this state. The elapsed time will be saved
// and restored as required.
//
// The `reason` argument is optional, as to
// give the user a reason why this test was paused,
// like "preparing input samples"
void tst_pause(const char *reason);

// Resumes a paused test. This method restarts any
// timers that was previously paused, if and only
// if they were paused before. 
// The `status` argument is optional, to notify
// the user about the state of the test after
// resume. This method can also be used just to
// update the status of a presently running process.
void tst_resume(const char *status);

// All test methods should return any nonzero number
// on return to denote success, and should return
// a zero to denote failure

#define TEST(name, method) \
    tst_start(name); \
    if(method) \
        tst_pass(); \
    else \
        tst_fail();
