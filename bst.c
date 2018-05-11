#include <stdlib.h>

#include "bst.h"
#include "arr.h"
#include "test.h"
#include "utils.h"
#include "sort.h"

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
        //if(value == tmp->value)
        //    return 0;
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

void bst_preorder(BinarySearchTree *bst, bst_process process){
    if(bst == NULL)
        return;
    process(bst->value);
    bst_preorder(bst->left, process);
    bst_preorder(bst->right, process);
}

void bst_inorder(BinarySearchTree *bst, bst_process process){
    if(bst == NULL)
        return;
    bst_inorder(bst->left, process);
    process(bst->value);
    bst_inorder(bst->right, process);
}

void bst_postorder(BinarySearchTree *bst, bst_process process){
    if(bst == NULL)
        return;
    bst_postorder(bst->left, process);
    bst_postorder(bst->right, process);
    process(bst->value);
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

static mint *bst_test_array;
static midx bst_test_pointer = 0;

static void test_bst_order_process(mint value){
    bst_test_array[bst_test_pointer] = value;
    bst_test_pointer++;
}

static mint test_bst_inorder(){
    bst_test_pointer = 0;
    bst_test_array = arr_new(BST_TEST_ITEM_COUNT);
    arr_fill_rand(bst_test_array, BST_TEST_ITEM_COUNT, 898213, SAMPLE_CASE_AVERAGE);
    
    BinarySearchTree  *bst = bst_create(bst_test_array, BST_TEST_ITEM_COUNT);
   
    bst_inorder(bst, test_bst_order_process);

    mint ret = check_sort(bst_test_array, BST_TEST_ITEM_COUNT, SORT_TYPE_ASCENDING);
    
    free(bst_test_array);
    bst_test_pointer = 0;
    bst_free(bst);

    return ret; 
}

static mint test_bst_preorder(){
    bst_test_pointer = 0;
    bst_test_array = arr_new(BST_TEST_ITEM_COUNT);
    arr_fill_rand(bst_test_array, BST_TEST_ITEM_COUNT, 898213, SAMPLE_CASE_BEST);
    
    BinarySearchTree  *bst = bst_create(bst_test_array, BST_TEST_ITEM_COUNT);
   
    bst_preorder(bst, test_bst_order_process);

    mint ret = check_sort(bst_test_array, BST_TEST_ITEM_COUNT, SORT_TYPE_ASCENDING);
    
    free(bst_test_array);
    bst_test_pointer = 0;
    bst_free(bst);

    return ret; 
}

static mint test_bst_postorder(){
    bst_test_pointer = 0;
    bst_test_array = arr_new(BST_TEST_ITEM_COUNT);
    arr_fill_rand(bst_test_array, BST_TEST_ITEM_COUNT, 898213, SAMPLE_CASE_BEST);
    
    BinarySearchTree  *bst = bst_create(bst_test_array, BST_TEST_ITEM_COUNT);
   
    bst_preorder(bst, test_bst_order_process);

    mint ret = check_sort(bst_test_array, BST_TEST_ITEM_COUNT, SORT_TYPE_DESCENDING);
    
    free(bst_test_array);
    bst_test_pointer = 0;
    bst_free(bst);

    return ret; 
}

void test_bst(){
    TEST("Binary Search Tree Creation", test_bst_create(BST_TEST_ITEM_COUNT));
    BinarySearchTree *bst = bst_test_gen;
    TEST("Binary Search Tree Searching", bst_search(bst, bst->left->value));
    TEST("Binary Search Tree Insertion", test_bst_insert(bst, random_at_most(BST_TEST_ITEM_COUNT)));
    bst_free(bst);
    TEST("Binary Search Tree Preorder", test_bst_preorder());
    TEST("Binary Search Tree Inorder", test_bst_inorder());
    TEST("Binary Search Tree Postorder", test_bst_preorder());
}
