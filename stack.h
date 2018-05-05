#pragma once

#include "common.h"

typedef struct{
    mint *values;
    mint *top;
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

void stack_free(Stack stack);

// No overflow check and/or stack expansion,
// use only when 200% sure about the state
// of the stack at runtime, otherwise bad
// things may happen, heap corruption being
// the minimum.
void stack_push_fast2(Stack stack, mint value);

#define stack_push_fast(stack, value) \
    do{ \
        *((Stack_Intr*)stack)->top = value; \
        ((Stack_Intr*)stack)->top++; \
    } while(0)

// Slower, safer push, with full overflow check
// and stack expansion support.
void stack_push(Stack stack, mint value);

// No underflow check, same cautions applicable
// as stack_push_fast
mint stack_pop_fast2(Stack stack);

#define stack_pop_fast(stack) (*(--((Stack_Intr*)stack)->top))

// Slower, safer pop, with full underflow check
mint stack_pop(Stack stack);


// Stack status query
mint stack_is_empty(Stack stack);
mint stack_is_overflow(Stack stack);
mint stack_is_underflow(Stack stack);
mint stack_size(Stack stack);

void test_stack();
