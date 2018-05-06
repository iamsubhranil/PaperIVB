#pragma once

#include "common.h"

#ifndef BST_TEST_ITEM_COUNT
#define BST_TEST_ITEM_COUNT 10000
#endif

typedef struct BinarySearchTree{
    mint value;
    struct BinarySearchTree *left;
    struct BinarySearchTree *right;
} BinarySearchTree;

BinarySearchTree *bst_create(mint *arr, midx n);
void bst_free(BinarySearchTree *bst);

mint bst_insert(BinarySearchTree *bst, mint value);

mint bst_search(BinarySearchTree *bst, mint value);

mint bst_delete(BinarySearchTree *bst, mint value);

typedef void(*bst_process)(mint);

void bst_preorder(BinarySearchTree *bst, bst_process process);
void bst_preorder_nonrec(BinarySearchTree *bst, bst_process process);

void bst_inorder(BinarySearchTree *bst, bst_process process);
void bst_inorder_nonrec(BinarySearchTree *bst, bst_process process);

void bst_postorder(BinarySearchTree *bst, bst_process process);
void bst_preorder_nonrec(BinarySearchTree *bst, bst_process process);

mint bst_count_leaves(BinarySearchTree *bst);
mint bst_count_internal(BinarySearchTree *bst);

void test_bst();
