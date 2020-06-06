#pragma once

#include "common.h"

typedef struct Avl *AVLTree;

AVLTree avl_create(i64 *arr, siz h);
void    avl_free(AVLTree avl);
u8      avl_search(AVLTree avl, i64 value);
u8      avl_insert(AVLTree *avl, i64 value);
u8      avl_delete(AVLTree *avl, i64 value);
void    avl_print(AVLTree avl);
i64     avl_height(AVLTree avl);

void test_avl();
