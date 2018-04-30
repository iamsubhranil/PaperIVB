#pragma once

#include <stdarg.h>

void tst_pass();
void tst_fail();
void tst_start(const char *name);
void tst_pause(const char *reason);
void tst_resume();

// All test methods should return any nonzero number
// on return to denote success, and should return
// a zero to denote failure

#define TEST(name, method) \
    tst_start(name); \
    if(method) \
        tst_pass(); \
    else \
        tst_fail();
