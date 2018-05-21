#pragma once

#include "common.h"

typedef struct{
    i64 a, b;
} Tuple;

typedef struct LinkedNode{
    i64 val;
    struct LinkedNode *next;
} LinkedNode;

i64 random_at_most(i64 range);
LinkedNode* linkednode_get(i64 value);
// Frees _ALL_ the linked nodes
void linkednode_free(LinkedNode *parent);
