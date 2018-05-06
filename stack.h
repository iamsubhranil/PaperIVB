#pragma once

#include "common.h"

typedef struct{
    union{
        mint *values;
        void **genericValues;
    };
    union{
        mint *top;
        void **genericTop;
    };
    midx count;
    mints grow;
    mints status; // 0-> Normal, 1-> Overflow, 2-> Underflow
} Stack_Intr;

typedef Stack_Intr* Stack;

// `grow` indicates whether the stack is
// permitted to grow dynamically at runtime.
// If `grow` is false, then when an overflow
// occurs, nothing will be pushed to the stack
// and the overflow flag will be set.
Stack stack_new(midx count, mints grow);
Stack stack_new_generic(midx count, mints grow);

void stack_free(Stack stack);
void stack_free_generic(Stack stack);

// No overflow check and/or stack expansion,
// use only when 200% sure about the state
// of the stack at runtime, otherwise bad
// things may happen, heap corruption being
// the minimum.
void stack_push_fast(Stack stack, mint value);
void stack_push_fast_generic(Stack stack, void *value);

// Slower, safer push, with full overflow check
// and stack expansion support.
void stack_push(Stack stack, mint value);
void stack_push_generic(Stack stack, void *value);

// No underflow check, same cautions applicable
// as stack_push_fast
mint stack_pop_fast(Stack stack);
void* stack_pop_fast_generic(Stack stack);

// Slower, safer pop, with full underflow check
mint stack_pop(Stack stack);
void* stack_pop_generic(Stack stack);


// Stack status query
mint stack_is_empty(Stack stack);
mint stack_is_overflow(Stack stack);
mint stack_is_underflow(Stack stack);
mint stack_size(Stack stack);
mint stack_size_generic(Stack stack);

void test_stack();
