#include "avl.h"

#include <stdio.h>
#include <stdlib.h>

#include "arr.h"
#include "display.h"
#include "test.h"
#include "utils.h"

typedef struct Avl {
	i64         value;
	struct Avl *left;
	struct Avl *right;
	i64         height;
} Avl;

static Avl *avl_get_node() {
	Avl *avl    = (Avl *)malloc(sizeof(Avl));
	avl->left   = NULL;
	avl->right  = NULL;
	avl->value  = 0;
	avl->height = 0;
	return avl;
}

static void avl_print2(Avl *avl, u8 indent, const char *side) {
	for(u8 j = 0; j < 2; j++) {
		pblue("\n");
		for(u8 i = 1; i <= indent; i++) {
			if(i % 5 == 0)
				pblue("|");
			else
				pblue(" ");
		}
	}
	if(indent > 0) {
		pblue("- %s : ", side);
		if(avl != NULL) {
			pgrn("%" Pi64, avl->value);
			if(avl->left || avl->right) {
				avl_print2(avl->left, indent + 5, "Left");
				avl_print2(avl->right, indent + 5, "Right");
			}
		} else
			pred("Null");
	} else {
		pcyn("Root : ");
		if(avl) {
			pgrn("%" Pi64, avl->value);
			avl_print2(avl->left, indent + 5, "Left");
			avl_print2(avl->right, indent + 5, "Right");
		} else
			pred("Null");
	}
}

void avl_print(Avl *avl) {
	avl_print2(avl, 0, "none");
}

i64 avl_height(Avl *root) {
	if(root == NULL)
		return 0;
	return root->height;
}

u8 avl_search(Avl *root, i64 val) {
	if(root == NULL)
		return 0;
	if(root->value == val)
		return 1;
	if(val > root->value)
		return avl_search(root->right, val);
	else
		return avl_search(root->left, val);
}

static i64 avl_find_height(Avl *root) {
	if(root == NULL)
		return 0;

	i64 lh = avl_height(root->left);
	i64 rh = avl_height(root->right);

	return ((lh > rh) ? lh : rh) + 1;
}

static Avl *avl_rotate_right(Avl *node) {
	Avl *l = node->left;

	node->left = l->right;
	l->right   = node;

	node->height = avl_find_height(node);
	l->height    = avl_find_height(l);

	return l;
}

static Avl *avl_rotate_left(Avl *node) {
	Avl *r = node->right;

	node->right = r->left;
	r->left     = node;

	node->height = avl_find_height(node);
	r->height    = avl_find_height(r);
	return r;
}

static i64 avl_get_balance(Avl *a) {
	if(a == NULL)
		return 0;
	return avl_height(a->left) - avl_height(a->right);
}

u8 avl_insert(Avl **root, i64 val) {
	Avl *avl = *root;
	if(avl == NULL) {
		*root          = avl_get_node();
		(*root)->value = val;
		return 1;
	}

	u8 ret = 0;

	if(val < avl->value)
		ret = avl_insert(&avl->left, val);
	else if(val > avl->value)
		ret = avl_insert(&avl->right, val);
	else
		return 0;
	if(ret == 0)
		return 0;

	avl->height = avl_find_height(avl);

	i64 balance = avl_get_balance(avl);

	if(balance > 1 && val < avl->left->value) {
		avl = avl_rotate_right(avl);
	} else if(balance < -1 && val > avl->right->value) {
		avl = avl_rotate_left(avl);
	} else if(balance > 1 && val > avl->left->value) {
		avl->left = avl_rotate_left(avl->left);
		avl       = avl_rotate_right(avl);
	} else if(balance < -1 && val < avl->right->value) {
		avl->right = avl_rotate_right(avl->right);
		avl        = avl_rotate_left(avl);
	}

	*root = avl;

	return 1;
}

u8 avl_delete(Avl **root, i64 val) {
	Avl *avl = *root;

	if(avl == NULL)
		return 0;

	if(val < avl->value)
		avl_delete(&avl->left, val);
	else if(val > avl->value)
		avl_delete(&avl->right, val);
	else {
		if(avl->left == NULL || avl->right == NULL) {
			Avl *tmp = avl->left ? avl->left : avl->right;
			*root    = tmp;

			free(avl);
			avl = tmp;
		} else {
			Avl *tmp = avl->right;
			while(tmp->left != NULL) tmp = tmp->left;
			avl->value = tmp->value;
			avl_delete(&avl->right, tmp->value);
		}
	}

	if(*root == NULL)
		return 1;

	avl->height = avl_find_height(avl);

	i64 balance = avl_get_balance(avl);

	if(balance > 1 && avl_get_balance(avl->left) >= 0)
		avl = avl_rotate_right(avl);
	else if(balance > 1 && avl_get_balance(avl->left) < 0) {
		avl->left = avl_rotate_left(avl->left);
		avl       = avl_rotate_right(avl);
	} else if(balance < -1 && avl_get_balance(avl->right) <= 0) {
		avl = avl_rotate_left(avl);
	} else if(balance < -1 && avl_get_balance(avl->right) > 0) {
		avl->right = avl_rotate_right(avl->right);
		avl        = avl_rotate_left(avl);
	}

	*root = avl;

	return 1;
}

Avl *avl_create(i64 *arr, siz n) {
	Avl *root = NULL;
	for(siz s = 0; s < n; s++) {
		avl_insert(&root, arr[s]);
	}
	return root;
}

void avl_free(Avl *avl) {
	if(avl == NULL)
		return;
	avl_free(avl->left);
	avl_free(avl->right);
	free(avl);
}

static Avl *test_avl_tree = NULL;

static u8 test_avl_create() {
	i64 *arr = arr_new(config.avl_test_item_count);
	// i64 *arr = arr_new(12);
	// arr_fill_int(arr, 12, 95, 20, 96, 8, 88, 14, 31, 75, 68, 100, 100, 1003);
	arr_fill_rand(arr, config.avl_test_item_count, 100, SAMPLE_CASE_AVERAGE);
	test_avl_tree = avl_create(arr, config.avl_test_item_count);
	// test_avl_tree = avl_create(arr, 12);
	free(arr);
	return test_avl_tree ? 1 : 0;
}

static i64 test_avl_insert(Avl **avl, i64 value) {
	avl_insert(avl, value);
	if(!avl_search(*avl, value))
		return 0;
	i64 nval = random_at_most(value);
	while(nval > 0) {
		i64 nval2 = random_at_most(value);
		avl_insert(avl, nval2);
		if(!avl_search(*avl, nval2))
			return 0;
		nval--;
	}
	return 1;
}

static i64 test_avl_delete(Avl **avl1) {
	// avl *avl = *avl1;
	i64 rootval = (*avl1)->value;
	avl_delete(avl1, rootval);
	if((*avl1)->value == rootval)
		return 0;
	Avl *ptr = *avl1;
	while(ptr->left && ptr->right) ptr = ptr->left;
	rootval = ptr->value;
	if(!avl_delete(avl1, rootval))
		return 0;
	ptr = *avl1;
	while(ptr->left || ptr->right) {
		if(ptr->right)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}
	rootval = ptr->value;
	if(!avl_delete(avl1, rootval))
		return 0;
	while((*avl1)->left || (*avl1)->right)
		if(!avl_delete(avl1, (*avl1)->value))
			return 0;
	if((*avl1)->left || (*avl1)->right)
		return 0;
	return 1;
}

void test_avl() {
	tst_suite_start("AVL Tree", 4);
	TEST("Creation", test_avl_create());
	TEST("Searching", avl_search(test_avl_tree, test_avl_tree->value));
	TEST("Insertion",
	     test_avl_insert(&test_avl_tree,
	                     random_at_most(config.avl_test_item_count)));
	TEST("Deletion", test_avl_delete(&test_avl_tree));
	avl_free(test_avl_tree);
	tst_suite_end();
}
