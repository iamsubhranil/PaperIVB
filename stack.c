#include <stdlib.h>

#include "stack.h"
#include "test.h"

Stack_Intr* stack_new(midx count){
    Stack_Intr *stack = (Stack_Intr*)malloc(sizeof(Stack_Intr));
    stack->count = count > 0 ? count : 0;
    stack->values = count == 0 ? NULL : (mint *)malloc(sizeof(mint) * count);
    stack->top = stack->values;
    return stack;
}

void stack_push(Stack_Intr *stack, mint value){
    if(stack->top == NULL || stack->top == stack->values + stack->count - 1){
        mint oldCount = stack->count;
        (stack->count == 0) ? (stack->count = 1) : (stack->count *= 2);
        stack->values = (mint *)realloc(stack->values, sizeof(mint) * stack->count);
        stack->top = stack->values + (oldCount == 0 ? 0 :oldCount - 1);
    }
    *(stack->top) = value;
    stack->top++;
}

mint stack_pop(Stack_Intr *stack){
    return *(--stack->top);
}

void stack_free(Stack_Intr *stack){
    free(stack->values);
    stack->count = 0;
    stack->top = stack->values = NULL;
    free(stack);
}

mint stack_empty(Stack_Intr *stack){
    return stack->top == stack->values;
}

// Tests
// =========================

static mint test_stack_new(midx count){
    Stack_Intr* s = stack_new(count);
    if(s->count == count && s->values != NULL){
        stack_free(s);
        return 1;
    }
    return 0;
}

static mint test_stack_push(Stack_Intr *stack){
    mint *bak = stack->top;
    stack_push(stack, 392);
    if(*(stack->top - 1) != 392)
        return 0;
    stack_push(stack, 8382);
    if(*(stack->top - 1) != 8382)
        return 0;
    stack->top = bak;
    return 1;
}

static mint test_stack_grow(Stack_Intr *stack){
    for(mint i = 0;i < 10000;i++)
        stack_push(stack, i);
    mint ret = 0;
    if(*(stack->top - 1) == 9999){
        ret = 1;
    }
    for(mint i = 0;i < 10000;i++)
        stack_pop(stack);
    return ret;
}

static mint test_stack_pop(Stack_Intr *stack){
    stack_push(stack, 372);
    if(stack_pop(stack) != 372)
        return 0;
    stack_push(stack, -382);
    if(stack_pop(stack) != -382)
        return 0;
    return 1;
}

static mint test_stack_empty(Stack_Intr *stack){
    stack_push(stack, 3832);
    if(stack_empty(stack))
        return 0;
    stack_pop(stack);
    return stack_empty(stack);
}

void test_stack(){
    TEST("Stack Creation", test_stack_new(10));
    Stack stack = stack_new(10);
    TEST("Stack Push", test_stack_push(stack));
    TEST("Stack Grow", test_stack_grow(stack));
    TEST("Stack Pop", test_stack_pop(stack));
    TEST("Stack Empty", test_stack_empty(stack));
    stack_free(stack);
}
