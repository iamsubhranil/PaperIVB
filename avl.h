#pragma once

#include "common.h"

#ifndef AVL_TEST_ITEM_COUNT
#define AVL_TEST_ITEM_COUNT 10000
#endif

typedef struct Avl* AVLTree;

u8 avl_insert(AVLTree *avl, i64 value);
void avl_print(AVLTree avl);
i64 avl_height(AVLTree avl);

void test_avl();