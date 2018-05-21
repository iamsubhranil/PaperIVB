#include <stdlib.h>
#include "utils.h"


// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
i64 random_at_most(i64 max) {
  i64
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = max + 1,
    num_rand = (i64) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  i64 x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (i64)x);

  // Truncated division is intentional
  return x/bin_size;
}

LinkedNode* linkednode_get(i64 value){
    LinkedNode *node = (LinkedNode *)malloc(sizeof(LinkedNode));
    if(!node)
        return NULL;
    node->next = NULL;
    node->val = value;
    return node;
}

void linkednode_free(LinkedNode *node){
    if(node == NULL)
        return;
    LinkedNode *bak = node->next;
    free(node);
    linkednode_free(bak);
}
