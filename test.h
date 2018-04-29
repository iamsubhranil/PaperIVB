#pragma once

#include <stdarg.h>

void tst_pass(const char *name);
void tst_fail(const char *name);
void tst_start(const char *name);

// All test methods should return any nonzero number
// on return to denote success, and should return
// a zero to denote failure

#define TEST(name, method) \
    tst_start(name); \
    if(method) \
        tst_pass(name); \
    else \
        tst_fail(name);
