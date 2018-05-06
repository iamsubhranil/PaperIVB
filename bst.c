#include <stdlib.h>

#include "bst.h"
#include "arr.h"
#include "test.h"

static BinarySearchTree* bst_new_node(){
    BinarySearchTree *node = (BinarySearchTree *)malloc(sizeof(BinarySearchTree));
    node->left = NULL;
    node->right = NULL;
    node->value = 0;
    return node;
}

mint bst_insert(BinarySearchTree *bst, mint value){
    BinarySearchTree *tmp = bst, *prev = bst;
    while(tmp != NULL){
        if(value == tmp->value)
            return 0;
        prev = tmp;
        if(value > tmp->value)
            tmp = tmp->right;
        else
            tmp = tmp->left;
    }
    if(value > prev->value){
        prev->right = bst_new_node();
        prev->right->value = value;
    }
    else{
        prev->left = bst_new_node();
        prev->left->value = value;
    }
    return 1;
}

BinarySearchTree *bst_create(mint *arr, midx n){
    BinarySearchTree *root = bst_new_node();
    root->value = arr[0];
    for(midx i = 1;i < n;i++)
        bst_insert(root, arr[i]);
    return root;
}

void bst_free(BinarySearchTree *bst){
    if(bst == NULL)
        return;
    bst_free(bst->left);
    bst_free(bst->right);
    free(bst);
}

mint bst_search(BinarySearchTree *bst, mint value){
    for(BinarySearchTree *tmp = bst;tmp != NULL;tmp = value > tmp->value ? tmp->right : tmp->left)
        if(tmp->value == value)
            return 1;
    return 0;
}

static mint test_bst_insert(BinarySearchTree *bst, mint value){
    bst_insert(bst, value);
    if(!bst_search(bst, value))
        return 0;
    mint nval = random_at_most(value);
    while(nval > 0){
        mint nval2 = random_at_most(value);
        bst_insert(bst, nval2);
        if(!bst_search(bst, nval2))
            return 0;
        nval--;
    }
    return 1;
}

static BinarySearchTree *bst_test_gen;

static mint test_bst_create(midx n){
    tst_pause("Generating random elements");
    mint *arr = arr_new(n);
    arr_fill_rand(arr, n, random_at_most(n), SAMPLE_CASE_AVERAGE);
    tst_resume("Creating BST");
    BinarySearchTree *bst = bst_create(arr, n);
    tst_resume("Cleaning up");
    arr_free(arr);
    if(!bst)
        return 0;
    bst_test_gen = bst;
    return 1;
}

void test_bst(){
    TEST("Binary Search Tree Creation", test_bst_create(BST_TEST_ITEM_COUNT));
    BinarySearchTree *bst = bst_test_gen;
    TEST("Binary Search Tree Searching", bst_search(bst, bst->left->value));
    TEST("Binary Search Tree Insertion", test_bst_insert(bst, random_at_most(BST_TEST_ITEM_COUNT)));
    bst_free(bst);
}
