#pragma once

#include "common.h"

#ifndef BST_TEST_ITEM_COUNT
#define BST_TEST_ITEM_COUNT 10000
#endif

typedef struct BST* BinarySearchTree;

BinarySearchTree bst_create(i64 *arr, siz n);
void bst_free(BinarySearchTree bst);

i64 bst_insert(BinarySearchTree bst, i64 value);

i64 bst_search(BinarySearchTree bst, i64 value);

i64 bst_delete(BinarySearchTree bst, i64 value);

typedef void(*bst_process)(i64);

void bst_preorder(BinarySearchTree bst, bst_process process);
void bst_preorder_nonrec(BinarySearchTree bst, bst_process process);

void bst_inorder(BinarySearchTree bst, bst_process process);
void bst_inorder_nonrec(BinarySearchTree bst, bst_process process);

void bst_postorder(BinarySearchTree bst, bst_process process);
void bst_preorder_nonrec(BinarySearchTree bst, bst_process process);

i64 bst_count_leaves(BinarySearchTree bst);
i64 bst_count_internal(BinarySearchTree bst);

void test_bst();
