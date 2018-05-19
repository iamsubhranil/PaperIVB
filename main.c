#include <stdio.h>

#include "arr.h"
#include "stack.h"
#include "sort.h"
#include "bst.h"
#include "queue.h"
#include "hashtable.h"
#include "avl.h"
#include "dump.h"

int main(){
    dump_init();
    test_arr();
    test_stack();
    test_queue();
    test_sort();
    test_bst();
    test_hashtable();
    test_avl();
    printf("\n");
}
