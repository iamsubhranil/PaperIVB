#pragma once

#include "common.h"

typedef struct Stack_Intr* Stack;

// `grow` indicates whether the stack is
// permitted to grow dynamically at runtime.
// If `grow` is false, then when an overflow
// occurs, nothing will be pushed to the stack
// and the overflow flag will be set.
Stack stack_new(siz count, u8 grow);
Stack stack_new_generic(siz count, u8 grow);

void stack_free(Stack stack);
void stack_free_generic(Stack stack);

// No overflow check and/or stack expansion,
// use only when 200% sure about the state
// of the stack at runtime, otherwise bad
// things may happen, heap corruption being
// the minimum.
void stack_push_fast(Stack stack, i64 value);
void stack_push_fast_generic(Stack stack, void *value);

// Slower, safer push, with full overflow check
// and stack expansion support.
void stack_push(Stack stack, i64 value);
void stack_push_generic(Stack stack, void *value);

// No underflow check, same cautions applicable
// as stack_push_fast
i64 stack_pop_fast(Stack stack);
void* stack_pop_fast_generic(Stack stack);

// Slower, safer pop, with full underflow check
i64 stack_pop(Stack stack);
void* stack_pop_generic(Stack stack);


// Stack status query
u8 stack_is_empty(Stack stack);
u8 stack_is_overflow(Stack stack);
u8 stack_is_underflow(Stack stack);
i64 stack_size(Stack stack);
i64 stack_size_generic(Stack stack);

void test_stack();
