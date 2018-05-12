#pragma once

#include "common.h"

typedef struct HT* HashTable;

typedef enum{
    HF_DIVISION,
    HF_MIDSQUARE,
    HF_FOLDING
} HashFunction;

typedef enum{
    PROBE_LINEAR,
    PROBE_QUADRATIC,
    PROBE_RANDOM
} ProbingTechnique;

HashTable hashtable_create_open(siz table_size, HashFunction func);
HashTable hashtable_create_closed(siz table_size, HashFunction func, ProbingTechnique prober);
void hashtable_free(HashTable table);

// table_size, value_to_hash
typedef siz (*hash_function)(siz, i64);

void hashtable_set_hashfunction(HashTable ht, hash_function hf);

u8 hashtable_insert(HashTable table, i64 value);
u8 hashtable_search(HashTable table, i64 value);
u8 hashtable_delete(HashTable table, i64 value);

void test_hashtable();
