#pragma once

#include "common.h"

typedef struct{
    mint *values;
    mint *top;
    midx count;
} Stack_Intr;

typedef Stack_Intr* Stack;

Stack stack_new(midx count);
void stack_push(Stack stack, mint value);
mint stack_pop(Stack stack);
void stack_free(Stack stack);
mint stack_empty(Stack stack);

void test_stack();
