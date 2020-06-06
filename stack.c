#include "stack.h"

#include <stdlib.h>

#include "arr.h"
#include "test.h"
#include "utils.h"

// Core data structure
// ===================

typedef struct Stack_Intr {
	union {
		i64 *  values;
		void **genericValues;
		u8 *   boolValues;
	};
	union {
		i64 *  top;
		void **genericTop;
		u8 *   boolTop;
	};
	siz count;
	u8  grow;
	u8  status; // 0-> Normal, 1-> Overflow, 2-> Underflow
} Stack_Intr;

// Type agnostic API
// =================

u8 stack_is_empty(Stack_Intr *stack) {
	return stack->top == stack->values;
}

u8 stack_is_overflow(Stack_Intr *stack) {
	return stack->status & 1;
}

u8 stack_is_underflow(Stack_Intr *stack) {
	return stack->status & 2;
}

// Type sensitive API
// ==================

#define stk_api(name_new, name_push, name_pushf, name_pop, name_popf,         \
                name_free, name_size, values, top, type, type_rand)           \
	Stack_Intr *stack_##name_new(siz count, u8 grow) {                        \
		Stack_Intr *stack = (Stack_Intr *)malloc(sizeof(Stack_Intr));         \
		stack->count      = count > 0 ? count : 0;                            \
		stack->values =                                                       \
		    count == 0 ? NULL : (type *)malloc(sizeof(type) * count);         \
		stack->top    = stack->values;                                        \
		stack->grow   = grow ? 1 : 0;                                         \
		stack->status = 0;                                                    \
		return stack;                                                         \
	}                                                                         \
                                                                              \
	void stack_##name_pushf(Stack_Intr *stack, type value) {                  \
		*(stack->top) = value;                                                \
		stack->top++;                                                         \
	}                                                                         \
                                                                              \
	void stack_##name_push(Stack_Intr *stack, type value) {                   \
		if(stack->top == NULL ||                                              \
		   stack->top == stack->values + stack->count - 1) {                  \
			if(!stack->grow) {                                                \
				stack->status |= 1;                                           \
				return;                                                       \
			}                                                                 \
			i64 oldCount = stack->count;                                      \
			(stack->count == 0) ? (stack->count = 1) : (stack->count *= 2);   \
			stack->values =                                                   \
			    (type *)realloc(stack->values, sizeof(type) * stack->count);  \
			stack->top = stack->values + (oldCount == 0 ? 0 : oldCount - 1);  \
		}                                                                     \
		*(stack->top) = value;                                                \
		stack->top++;                                                         \
		if(stack->status & 2)                                                 \
			stack->status = 0;                                                \
	}                                                                         \
                                                                              \
	type stack_##name_popf(Stack_Intr *stack) { return *(--stack->top); }     \
                                                                              \
	type stack_##name_pop(Stack_Intr *stack) {                                \
		if(stack->top == stack->values) {                                     \
			stack->status |= 2;                                               \
			return 0;                                                         \
		}                                                                     \
		if(stack->status & 1)                                                 \
			stack->status = 0;                                                \
		return *(--stack->top);                                               \
	}                                                                         \
                                                                              \
	void stack_##name_free(Stack_Intr *stack) {                               \
		free(stack->values);                                                  \
		stack->count = 0;                                                     \
		stack->top = stack->values = NULL;                                    \
		free(stack);                                                          \
	}                                                                         \
                                                                              \
	i64 stack_##name_size(Stack_Intr *stack) {                                \
		return stack->top - stack->values;                                    \
	}                                                                         \
                                                                              \
	static i64 test_stack_##name_new(siz count) {                             \
		Stack_Intr *s = stack_##name_new(count, 0);                           \
		if(s->count == count && s->values != NULL) {                          \
			stack_##name_free(s);                                             \
			return 1;                                                         \
		}                                                                     \
		return 0;                                                             \
	}                                                                         \
                                                                              \
	static i64 test_stack_##name_push(Stack_Intr *stack) {                    \
		i64  oldsize = stack_##name_size(stack);                              \
		type randel  = type_rand();                                           \
		stack_##name_push(stack, randel);                                     \
		if(*(stack->top - 1) != randel)                                       \
			return 0;                                                         \
		randel = type_rand();                                                 \
		stack_##name_push(stack, randel);                                     \
		if(*(stack->top - 1) != randel)                                       \
			return 0;                                                         \
		stack->top = stack->values + oldsize;                                 \
		return 1;                                                             \
	}                                                                         \
                                                                              \
	static i64 test_stack_grow_##name_push(Stack_Intr *stack) {               \
		for(i64 i = 0; i < 10000; i++) stack_##name_push(stack, type_rand()); \
		i64 ret = 0;                                                          \
		if(stack->top - stack->values == 10000) {                             \
			ret = 1;                                                          \
		}                                                                     \
		stack->top -= 10000;                                                  \
		return ret;                                                           \
	}                                                                         \
                                                                              \
	static i64 test_stack_##name_pop(Stack_Intr *stack) {                     \
		for(i64 i = 0; i < 10000; i++) {                                      \
			type randel = type_rand();                                        \
			stack_##name_push(stack, randel);                                 \
			if(stack_##name_pop(stack) != randel)                             \
				return 0;                                                     \
		}                                                                     \
		return 1;                                                             \
	}                                                                         \
                                                                              \
	static i64 test_stack_##name_size(Stack_Intr *stack) {                    \
		stack->top = stack->values;                                           \
		i64 size   = 4389;                                                    \
		for(i64 i = 0; i < size; i++) stack_##name_push(stack, type_rand());  \
		if(stack_##name_size(stack) != size)                                  \
			return 0;                                                         \
		stack->top = stack->values;                                           \
		return 1;                                                             \
	}                                                                         \
                                                                              \
	static i64 test_stack_empty_##name_pop(Stack_Intr *stack) {               \
		stack->top = stack->values;                                           \
		stack_##name_push(stack, type_rand());                                \
		if(stack_is_empty(stack))                                             \
			return 0;                                                         \
		stack_##name_pop(stack);                                              \
		return stack_is_empty(stack);                                         \
	}                                                                         \
                                                                              \
	static i64 test_stack_overflow_##name_push(Stack_Intr *stack) {           \
		for(i64 i = stack_##name_size(stack); i < stack->count + 1; i++)      \
			stack_##name_push(stack, type_rand());                            \
		return stack_is_overflow(stack);                                      \
	}                                                                         \
                                                                              \
	static i64 test_stack_underflow_##name_pop(Stack_Intr *stack) {           \
		for(i64 i = stack_##name_size(stack); i > -1; i--)                    \
			stack_##name_pop(stack);                                          \
		return stack_is_underflow(stack);                                     \
	}

static i64 rand_int() {
	return random_at_most(rand());
}

stk_api(new, push, push_fast, pop, pop_fast, free, size, values, top, i64,
        rand_int);

static void *rand_void() {
	return (void *)random();
}

stk_api(new_generic, push_generic, push_fast_generic, pop_generic,
        pop_fast_generic, free_generic, size_generic, genericValues, genericTop,
        void *, rand_void);

static u8 rand_bool() {
	return random_at_most(1);
}

stk_api(new_bool, push_bool, push_fast_bool, pop_bool, pop_fast_bool, free_bool,
        size_bool, boolValues, boolTop, u8, rand_bool);
#undef stk_api

// Tests
// =====

void test_stack() {
#define stk_tst(name, name_new, name_push, name_pushf, name_pop, name_popf,  \
                name_size, name_free)                                        \
	tst_suite_start("Stack (Type : " #name ")", 8);                          \
	TEST("Creation", test_stack_##name_new(config.stack_test_item_count));   \
	Stack stack_##name  = stack_##name_new(config.stack_test_item_count, 1); \
	Stack stack2_##name = stack_##name_new(config.stack_test_item_count, 0); \
	TEST("Push", test_stack_##name_push(stack_##name));                      \
	TEST("Overflow", test_stack_overflow_##name_push(stack2_##name));        \
	TEST("Grow", test_stack_grow_##name_push(stack_##name));                 \
	TEST("Pop", test_stack_##name_pop(stack_##name));                        \
	TEST("Underflow", test_stack_underflow_##name_pop(stack2_##name));       \
	TEST("Size", test_stack_##name_size(stack_##name));                      \
	TEST("Empty", test_stack_empty_##name_pop(stack_##name));                \
	stack_##name_free(stack_##name);                                         \
	stack_##name_free(stack2_##name);                                        \
	tst_suite_end();

	stk_tst(Integer, new, push, push_fast, pop, pop_fast, size, free);
	stk_tst(Generic, new_generic, push_generic, push_fast_generic, pop_generic,
	        pop_fast_generic, size_generic, free_generic);
	stk_tst(Boolean, new_bool, push_bool, push_fast_bool, pop_bool,
	        pop_fast_bool, size_bool, free_bool);

#undef stk_tst
}
