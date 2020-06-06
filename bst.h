#pragma once

#include "common.h"

typedef struct BST {
	i64         value;
	struct BST *left;
	struct BST *right;
} BST;

typedef struct BST *BinarySearchTree;

BinarySearchTree bst_create(i64 *arr, siz n);
void             bst_free(BinarySearchTree bst);

u8 bst_insert(BinarySearchTree bst, i64 value);

u8 bst_search(BinarySearchTree bst, i64 value);

// Root can also be modified
u8 bst_delete(BinarySearchTree *bst, i64 value);

typedef void (*bst_process)(i64);

void bst_preorder(BinarySearchTree bst, bst_process process);
void bst_preorder_nonrec(BinarySearchTree bst, bst_process process);

void bst_inorder(BinarySearchTree bst, bst_process process);
void bst_inorder_nonrec(BinarySearchTree bst, bst_process process);

void bst_postorder(BinarySearchTree bst, bst_process process);
void bst_preorder_nonrec(BinarySearchTree bst, bst_process process);

i64 bst_count_leaves(BinarySearchTree bst);
i64 bst_count_internal(BinarySearchTree bst);
i64 bst_count_nodes(BinarySearchTree bst);
i64 bst_find_height(BinarySearchTree bst);

void bst_print(BinarySearchTree bst);

void test_bst();
