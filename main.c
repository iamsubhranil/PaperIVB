#include <stdio.h>
#include <string.h>

#include "CargParser/cargparser.h"
#include "arr.h"
#include "avl.h"
#include "bst.h"
#include "display.h"
#include "dump.h"
#include "graph.h"
#include "hashtable.h"
#include "numerical.h"
#include "queue.h"
#include "sort.h"
#include "stack.h"

Configuration config = {
    .counts =
        {
            ARRAY_TEST_ITEM_COUNT,     // arr_test_item_count;
            AVL_TEST_ITEM_COUNT,       // avl_test_item_count;
            BST_TEST_ITEM_COUNT,       // bst_test_item_count;
            0,                         // graph
            HASHTABLE_TEST_ITEM_COUNT, // hashtable_test_item_count;
            0,                         // numericals
            QUEUE_TEST_ITEM_COUNT,     // queue_test_item_count;
            SORT_TEST_ITEM_COUNT,      // sort_test_item_count;
            STACK_TEST_ITEM_COUNT      // stack_test_item_count;
        },
    .tests =
        {
            true, // arr_test;
            true, // avl_test;
            true, // bst_test;
            true, // graph_test;
            true, // graph_test_floyd_warshall;
            true, // hashtable_test;
            true, // numerical_test;
            true, // queue_test;
            true, // sort_test;
        },
    .sort_tests =
        {
            true, // sort_test_bubble;
            true, // sort_test_bubble_rec;
            true, // sort_test_bubble_adv;
            true, // sort_test_insertion;
            true, // sort_test_insertion_rec;
            true, // sort_test_quick;
            true, // sort_test_quick_nonrec;
            true, // sort_test_heap;
            true, // sort_test_merge;
            true, // sort_test_merge_nonrec;
            true, // sort_test_radix;
            true, // sort_test_selection;
            true, // sort_test_selection_rec;
        },
#ifdef SORT_ENABLE_VISUAL
    .sort_enable_visual = false, // sort_enable_visual;
#endif
};

const char *testnames[] = {"array",     "avl",   "bst",  "graph", "hashtable",
                           "numerical", "queue", "sort", "stack"};

const char *sorting_algorithms[] = {
    "bubble",        "bubble_rec", "bubble_adv",   "heap",  "insertion",
    "insertion_rec", "merge",      "merge_nonrec", "quick", "quick_nonrec",
    "radix",         "selection",  "selection_rec"};

static void usage(const char *name) {
	printf("Usage:\n");
	printf("\t%s -t=<test_name> [-c=<sample_count] [-a=<algorithm>] [-v]\n",
	       name);
	printf("<test_name> is one of: %s", testnames[0]);
	for(int i = 1; i < 9; i++) printf(", %s", testnames[i]);
	printf("\n");
	printf("If '-t' is absent, all tests are performed.\n");
	printf("<sample_count> denotes the number of samples for the specified "
	       "test.\n");
	printf("By default, each test has a specific sample count.\n");
	printf(
	    "<algorithm> is only available for sort test, and can be one of the\n");
	printf("following: \n");
	printf("%s", sorting_algorithms[0]);
	for(int i = 1; i < 13; i++) printf(", %s", sorting_algorithms[i]);
	printf("\nIf not mentioned, all algorithms are tested.\n");
	printf("'-v' is only available for sort test, and when enabled, makes\n");
	printf("the sorting algorithm draw a visual representation of the sort.\n");
}

static int expect_oneof(char s, ArgumentList list, const char *errmsg,
                        const char *argv0, int count, const char **args) {
	if(arg_is_present(list, s)) {
		char *val = arg_value(list, s);
		if(val != NULL) {
			for(int i = 0; i < count; i++) {
				if(strcmp(args[i], val) == 0)
					return i;
			}
		}
		if(val != NULL)
			err("Wrong value for argument '-%c'!", s);
		else
			err("Argument '-%c' should not be empty!", s);
		err("Expected one of : %s", args[0]);
		for(int i = 1; i < count; i++) printf(", %s", args[i]);
		printf("\n");
		exit(1);
	} else {
		err("%s!", errmsg);
		arg_free(list);
		usage(argv0);
		exit(1);
	}
}

static int parse_int(const char *str, ArgumentList list, const char *argv0) {
	char *end   = NULL;
	int   value = strtol(str, &end, 10);
	if(*end != 0 || value < 0) {
		err("Invalid positive integer '%s'!", str);
		arg_free(list);
		usage(argv0);
		exit(1);
	}
	return value;
}

static void get_int_impl(char s, int *slot, const char *name, ArgumentList list,
                         char *argv0, bool isoptional, int defaultValue) {
	if(arg_is_present(list, s)) {
		*slot = parse_int(arg_value(list, s), list, argv0);
	} else if(isoptional) {
		*slot = defaultValue;
	} else {
		err("Expected argument '-%c' (%s)!", s, name);
		arg_free(list);
		usage(argv0);
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	ArgumentList list = arg_list_create(5);

	arg_add(list, 'h', "help", false);
	// by default, we enable all tests
	arg_add(list, 't', "test", true);
	arg_add(list, 'c', "count", true);
	arg_add(list, 'a', "algo", true);    // for sorting
	arg_add(list, 'v', "visual", false); // for sorting

	arg_parse(argc, &argv[0], list);

	if(arg_is_present(list, 'h')) {
		usage(argv[0]);
		return 0;
	}

	if(arg_is_present(list, 't')) {
		int testid = expect_oneof('t', list, "Invalid test name!", argv[0], 9,
		                          &testnames[0]);
		int count;
		get_int_impl('c', &count, "sample count", list, argv[0], true, -1);
		for(int i = 0; i < 9; i++) config.tests[i] = false;
		config.tests[testid] = true;
		if((testid == 3 || testid == 5) && count != -1) {
			err("Invalid option '--count' for selected test!");
			return 2;
		}
		if(count != -1)
			config.counts[testid] = count;
		if(testid != 7) {
			if(arg_is_present(list, 'a') || arg_is_present(list, 'v')) {
				err("Only '--test=sort' requires '--algo' and/or '--visual'!");
				return 3;
			}
		} else {
			// sorting
			if(arg_is_present(list, 'v')) {
#ifdef SORT_ENABLE_VISUAL
				config.sort_enable_visual = true;
#else
				err("Visual sorting is disabled! Compile with "
				    "'-DSORT_ENABLE_VISUAL' and run again!");
				return 4;
#endif
			}
			if(arg_is_present(list, 'a')) {
				int sortid =
				    expect_oneof('a', list, "Invalid sorting algorithm!",
				                 argv[0], 13, &sorting_algorithms[0]);
				for(int i = 0; i < 13; i++) config.sort_tests[i] = false;
				config.sort_tests[sortid] = true;
			}
		}
	} else {
		// if '--test' is not present, but other flags are,
		// then it's an error
		if(arg_is_present(list, 'c') || arg_is_present(list, 'a') ||
		   arg_is_present(list, 'v')) {
			err("Argument requires '--test'!");
			return 1;
		}
	}

	arg_free(list);

	dump_init();
	if(config.arr_test)
		test_arr();
	if(config.avl_test)
		test_avl();
	if(config.bst_test)
		test_bst();
	if(config.graph_test)
		test_graph();
	if(config.hashtable_test)
		test_hashtable();
	if(config.numerical_test)
		test_numericals();
	if(config.queue_test)
		test_queue();
	if(config.sort_test)
		test_sort();
	if(config.stack_test)
		test_stack();
	printf("\n");
	return 0;
}
