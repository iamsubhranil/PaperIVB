#include <stdlib.h>

#include "bst.h"
#include "arr.h"
#include "test.h"
#include "utils.h"
#include "sort.h"
#include "stack.h"
#include "display.h"

static BST* bst_new_node(){
    BST *node = (BST *)malloc(sizeof(BST));
    node->left = NULL;
    node->right = NULL;
    node->value = 0;
    return node;
}

u8 bst_insert(BST *bst, i64 value){
    BST *tmp = bst, *prev = bst;
    while(tmp){
        prev = tmp;
        if(value > tmp->value)
            tmp = tmp->right;
        else if(value < tmp->value)
            tmp = tmp->left;
        else
            return 0;
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

BST *bst_create(i64 *arr, siz n){
    BST *root = bst_new_node();
    root->value = arr[0];
    for(siz i = 1;i < n;i++)
        bst_insert(root, arr[i]);
    return root;
}

i64 bst_count_nodes(BST *bst){
    if(bst == NULL)
        return 0;
    return bst_count_nodes(bst->left) + bst_count_nodes(bst->right) + 1;
}

i64 bst_count_leaves(BST *bst){
    if(bst == NULL)
        return 0;
    if(bst->left || bst->right)
        return bst_count_leaves(bst->left) + bst_count_leaves(bst->right);
    return 1;
}

i64 bst_count_internal(BST *bst){
    if(bst == NULL)
        return 0;
    if(bst->left || bst->right)
        return bst_count_internal(bst->right) + bst_count_internal(bst->left) + 1;
    return 0;
}

i64 bst_find_height(BST *bst){
    if(bst == NULL)
        return 0;
    if(!bst->left && !bst->right)
        return 0;
    i64 lh = bst_find_height(bst->left);
    i64 rh = bst_find_height(bst->right);
    return (lh > rh ? lh : rh) + 1;
}

BST* inorder_successor(BST *root, BST *ptr){
    if(ptr->right){
        ptr = ptr->right;
        while(ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }
    BST *suc = NULL;
    while(root){
        if(root->value > ptr->value){
            suc = root;
            root = root->left;
        }
        else if(root->value < ptr->value)
            root = root->right;
        else
            break;
    }
    return suc;
}

u8 bst_delete(BST **root, i64 val){
    //bst_print(*root);
    u8 found = 0;
    BST *ptr = *root, *prev = NULL;
    while(ptr){
        if(ptr->value == val){
            found = 1;
            break;
        }
        prev = ptr;
        if(ptr->value < val)
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }
    if(!found)
        return 0;

    u8 delete_type = 0;
    if(!ptr->left && !ptr->right)
        delete_type = 1;
    else if(ptr->left && ptr->right)
        delete_type = 3;
    else
        delete_type = 2;

    switch(delete_type){
        case 1:
            {
                if(prev == NULL)
                    *root = NULL;
                else if(prev->right == ptr)
                    prev->right = NULL;
                else
                    prev->left = NULL;
                free(ptr);
            }
            break;
        case 2:
            {
                BST *child = ptr->left == NULL ?
                    ptr->right : ptr->left;
                if(prev == NULL)
                    *root = child;
                else if(prev->left == ptr)
                    prev->left = child;
                else
                    prev->right = child;
                free(ptr);
            }
            break;
        case 3:
            {
                BST *insuc = inorder_successor(*root, ptr);
                i64 val1 = insuc->value;
                bst_delete(root, val1);
                ptr->value = val1;
            }
            break;
    }
    return 1;
}

void bst_free(BST *bst){
    if(bst == NULL)
        return;
    bst_free(bst->left);
    bst_free(bst->right);
    free(bst);
}

u8 bst_search(BST *bst, i64 value){
    for(BST *tmp = bst;tmp != NULL;tmp = value > tmp->value ? tmp->right : tmp->left)
        if(tmp->value == value)
            return 1;
    return 0;
}

void bst_preorder(BST *bst, bst_process process){
    if(bst == NULL)
        return;
    process(bst->value);
    bst_preorder(bst->left, process);
    bst_preorder(bst->right, process);
}

void bst_preorder_nonrec(BST *bst, bst_process process){
    BST *ptr = bst;
    Stack s = stack_new_generic(1, 1);
    stack_push_generic(s, ptr);
    while(!stack_is_empty(s)){
        BST *cur = (BST *)stack_pop_generic(s);
        process(cur->value);

        if(cur->right)
            stack_push_generic(s, cur->right);
        if(cur->left)
            stack_push_generic(s, cur->left);
    }
    stack_free_generic(s);
}

void bst_inorder(BST *bst, bst_process process){
    if(bst == NULL)
        return;
    bst_inorder(bst->left, process);
    process(bst->value);
    bst_inorder(bst->right, process);
}

void bst_inorder_nonrec(BST *bst, bst_process process){
    BST *ptr = bst;
    Stack s = stack_new_generic(1, 1);
    do{
        while(ptr != NULL){
            stack_push_generic(s, ptr);
            ptr = ptr->left;
        }
        ptr = (BST *)stack_pop_generic(s);
        process(ptr->value);
        ptr = ptr->right;
    } while(!stack_is_empty(s) || ptr != NULL);
    stack_free_generic(s);
}

void bst_postorder(BST *bst, bst_process process){
    if(bst == NULL)
        return;
    bst_postorder(bst->left, process);
    bst_postorder(bst->right, process);
    process(bst->value);
}

void bst_postorder_nonrec(BST *bst, bst_process process){
    BST *ptr = bst;
    Stack s = stack_new_generic(1, 1);
    Stack b = stack_new_bool(1, 1);
    do{
        while(ptr != NULL){
            stack_push_generic(s, ptr);
            stack_push_bool(b, 0);
            ptr = ptr->left;
        }
        u8 already_seen = stack_pop_bool(b);
        BST *cur = (BST *)stack_pop_generic(s);
        if(already_seen){
            process(cur->value);
        }
        else{
            stack_push_bool(b, 1);
            stack_push_generic(s, cur);
            ptr = cur->right;
        }
    } while(!stack_is_empty(s) || ptr != NULL);
    stack_free_generic(s);
    stack_free_bool(b);
}

static void bst_print2(BST *bst, u8 indent, const char *side){
    for(u8 j = 0;j < 2;j++){
        pblue("\n");
        for(u8 i = 1;i <= indent;i++){
            if(i % 5 == 0)
                pblue("|");
            else
                pblue(" ");
        }
    }
    if(indent > 0){
        pblue("- %s : ", side);
        if(bst != NULL){
            pgrn("%" Pi64, bst->value);
            bst_print2(bst->left, indent + 5, "Left");
            bst_print2(bst->right, indent + 5, "Right");
        }
        else
            pred("Null");
    }
    else{
        pcyn("Root : ");
        if(bst){
            pgrn("%" Pi64, bst->value);
            bst_print2(bst->left, indent + 5, "Left");
            bst_print2(bst->right, indent + 5, "Right");
        }
        else
            pred("Null");
    }
    
}

void bst_print(BST *bst){
    bst_print2(bst, 0, "none");
}

static BST *bst_test_gen;

static i64 test_bst_create(siz n){
    tst_pause("Generating random elements");
    i64 *arr = arr_new(n);
    arr_fill_rand(arr, n, 100000, SAMPLE_CASE_AVERAGE);
    tst_resume("Creating BST");
    BST *bst = bst_create(arr, n);
    tst_resume("Cleaning up");
    arr_free(arr);
    if(!bst)
        return 0;
    bst_test_gen = bst;
    return 1;
}

static i64 test_bst_insert(BST *bst, i64 value){
    bst_insert(bst, value);
    if(!bst_search(bst, value))
        return 0;
    i64 nval = random_at_most(value);
    while(nval > 0){
        i64 nval2 = random_at_most(value);
        bst_insert(bst, nval2);
        if(!bst_search(bst, nval2))
            return 0;
        nval--;
    }
    return 1;
}

static i64 test_bst_delete(BST **bst1){
   // BST *bst = *bst1;
    i64 rootval = (*bst1)->value;
    bst_delete(bst1, rootval);
    if((*bst1)->value == rootval)
        return 0;
    BST *ptr = *bst1;
    while(ptr->left && ptr->right)
        ptr = ptr->left;
    rootval = ptr->value;
    if(!bst_delete(bst1, rootval))
        return 0;
    ptr = *bst1;
    while(ptr->left || ptr->right){
        if(ptr->right)
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }
    rootval = ptr->value;
    if(!bst_delete(bst1, rootval))
        return 0;
    while((*bst1)->left || (*bst1)->right)
        if(!bst_delete(bst1, (*bst1)->value))
            return 0;
    if((*bst1)->left || (*bst1)->right)
        return 0;
    return 1;
}

static i64 *bst_test_array;
static siz bst_test_pointer = 0;

static void test_bst_order_process(i64 value){
    bst_test_array[bst_test_pointer] = value;
    bst_test_pointer++;
}

static BST *bst_order_test_tree = NULL;

#define bst_order_test(ordername, output_order) \
static i64 test_bst_##ordername(){ \
    bst_test_pointer = 0; \
    arr_fill_rand(bst_test_array, BST_TEST_ITEM_COUNT, 3289983, SAMPLE_CASE_AVERAGE); \
    bst_##ordername(bst_order_test_tree, test_bst_order_process); \
    i64 ret = check_sort(bst_test_array, bst_count_nodes(bst_order_test_tree), SORT_TYPE_##output_order); \
    return ret;  \
}

bst_order_test(inorder, ASCENDING)
bst_order_test(inorder_nonrec, ASCENDING)
bst_order_test(preorder, ASCENDING)
bst_order_test(preorder_nonrec, ASCENDING)
bst_order_test(postorder, DESCENDING)
bst_order_test(postorder_nonrec, DESCENDING)


#undef bst_order_test

#define bst_count_test(name, value)\
static i64 test_bst_count_##name(){ \
    if(bst_order_test_tree == NULL) { \
        bst_test_pointer = 0; \
        bst_test_array = arr_new(BST_TEST_ITEM_COUNT); \
        tst_pause("Creating binary tree"); \
        arr_fill_rand(bst_test_array, BST_TEST_ITEM_COUNT, 20003, SAMPLE_CASE_AVERAGE); \
        i64 *bst_input_array = arr_new(BST_TEST_ITEM_COUNT); \
        arr_fill_rand(bst_input_array, BST_TEST_ITEM_COUNT, 898213, SAMPLE_CASE_BEST); \
        bst_order_test_tree = bst_create(bst_input_array, BST_TEST_ITEM_COUNT); \
        free(bst_input_array); \
        tst_resume("Couting"); \
    } \
    i64 ret = bst_count_##name(bst_order_test_tree) == value; \
    return ret; \
}

bst_count_test(nodes, BST_TEST_ITEM_COUNT)
bst_count_test(internal, BST_TEST_ITEM_COUNT - 1)
bst_count_test(leaves, 1)

#undef bst_count_test

static i64 test_bst_find_height(){
    tst_resume("Finding height");
    if(bst_find_height(bst_order_test_tree) != BST_TEST_ITEM_COUNT - 1)
        return 0;
    return 1;
}

void test_bst(){
    tst_suite_start("Binary Search Tree", 14);
    TEST("Creation", test_bst_create(BST_TEST_ITEM_COUNT));
    BST *bst = bst_test_gen;
    TEST("Searching", bst_search(bst, bst->value));
    TEST("Insertion", test_bst_insert(bst, random_at_most(BST_TEST_ITEM_COUNT)));
    TEST("Deletion", test_bst_delete(&bst));
    bst_free(bst);
    TEST("Total Node Count", test_bst_count_nodes());
    TEST("Internal Node Count", test_bst_count_internal());
    TEST("Leaf Node Count", test_bst_count_leaves());
    TEST("Height", test_bst_find_height());
    TEST("Preorder", test_bst_preorder());
    TEST("Preorder Non Recursive", test_bst_preorder_nonrec());
    TEST("Inorder", test_bst_inorder());
    TEST("Inorder Non Recursive", test_bst_inorder_nonrec());
    TEST("Postorder", test_bst_postorder());
    TEST("Postorder Non Recursive", test_bst_postorder_nonrec());
    free(bst_test_array);
    bst_free(bst_order_test_tree);
    tst_suite_end();
}
