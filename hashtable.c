#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "utils.h"
#include "test.h"
#include "arr.h"

typedef enum{
    TABLE_OPEN,
    TABLE_CLOSED
} TableType;

typedef siz (*hash_prober)(HashTable table, siz generated_index);
typedef u8 (*inserter)(HashTable table, i64 value);
typedef u8 (*searcher)(HashTable table, i64 value);
typedef void (*releaser)(HashTable table);

typedef struct HT{
    union{
        i64 *table;
        LinkedNode **openTable;
    };
    u8 *used;
    siz table_size;
    TableType type;
    hash_function func;
    hash_prober prober;
    inserter insert;
    searcher search;
    releaser free;
} HT;

static siz hf_division(siz table_size, i64 value){
    value = value - ((value / table_size) * table_size);
    return value;
}

static siz hf_midsq(siz table_size, i64 value){
    i64 sq = value * value;
    i64 d = 1, res = 0;
    while(sq > 0){
        i64 dig = sq - ((sq / 10) * 10);
        res *= 10;
        res += dig;
        d *= 100;
        sq /= d;
    }
    return res % table_size;
}

static i64 rev_two_dig(i64 input){
    i64 dig1 = input - ((input / 10) * 10);
    i64 dig2 = input / 10;
    return dig1 * 10 + dig2;
}

static siz hf_folding(siz table_size, i64 value){
    i64 res = 0;
    u8 flag = 0;
    // Fold shifting
    while(value > 0){
        i64 two = value - ((value / 100) * 100);
        if(flag){
            res += rev_two_dig(two);
            flag = 0;
        }
        else{
            res += two;
            flag = 1;
        }
        value /= 100;
    }
    return res % table_size;
}

static siz hp_linear(HT *table, siz gen_index){
    siz orig = gen_index;
    while(table->used[gen_index]){
        gen_index = (gen_index + 1) == table->table_size ? 0 : (gen_index + 1);
        if(orig == gen_index)
            return orig;
    }
    return gen_index;
}

static siz hp_quadratic(HT *table, siz gen_index){
    siz orig = gen_index;
    i64 i = 1;
    while(table->used[gen_index]){
        gen_index = (orig + (i*i));
        while(gen_index >= table->table_size)
            gen_index -= table->table_size;

        if(orig == gen_index)
            return orig;
        i++;
    }
    return gen_index;
}

static siz hp_random(HT *table, siz gen_index){
    siz orig = gen_index;
    i64 m = random_at_most(random()), h = random_at_most(table->table_size);
    while((m > h ? m/h : h/m) > 1)
        h = random_at_most(table->table_size);
    siz idx = orig;
    do{
        idx = ((idx + m) % h) + 1;
        if(idx == orig)
            return orig;
    } while(table->used[idx]);
    return idx;
}

static void hashtable_free_open(HT *ht){
    for(siz i = 0;i < ht->table_size;i++){
        if(ht->used[i]){
            LinkedNode *bak = ht->openTable[i];
            while(bak != NULL){
                LinkedNode *prev = bak;
                bak = bak->next;
                free(prev);
            }
        }
    }
    free(ht->openTable);
    free(ht->used);
    free(ht);
}

static u8 hashtable_search_open(HT *ht, i64 value){
    siz orig = ht->func(ht->table_size, value);
    if(ht->used[orig]){
        LinkedNode *tmp = ht->openTable[orig];
        while(tmp != NULL){
            if(tmp->val == value)
                return 1;
            tmp = tmp->next;
        }
    }
    return 0;
}

static u8 hashtable_insert_open(HT *ht, i64 value){
    if(hashtable_search_open(ht, value))
        return 0;
    siz idx = ht->func(ht->table_size, value);
    if(ht->used[idx]){
        LinkedNode *p = ht->openTable[idx];
        while(p->next != NULL)
            p = p->next;
        p->next = linkednode_get(value);
    }
    else{
        ht->used[idx] = 1;
        ht->openTable[idx] = linkednode_get(value);
    }
    return 1;
}

HT* hashtable_create_open(siz size, HashFunction fun){
    HT *ht = (HT *)malloc(sizeof(HT));
    ht->table_size = size;
    ht->openTable = (LinkedNode **)malloc(sizeof(LinkedNode*) * size);
    ht->used = (u8 *)malloc(sizeof(u8) * size);
    ht->type = TABLE_OPEN;
    memset(ht->used, 0, sizeof(u8) * size);
    switch(fun){
        case HF_DIVISION:
            ht->func = hf_division;
            break;
        case HF_FOLDING:
            ht->func = hf_folding;
            break;
        default:
            ht->func = hf_midsq;
    }
    ht->insert = hashtable_insert_open;
    ht->search = hashtable_search_open;
    ht->free = hashtable_free_open;
    return ht;
}

static void hashtable_free_closed(HT *ht){
    free(ht->table);
    free(ht->used);
    free(ht);
}

void hashtable_set_hashfunction(HT *ht, hash_function fun){
    ht->func = fun;
}

static u8 hashtable_search_closed(HT *ht, i64 value){
    siz orig = ht->func(ht->table_size, value);
    siz idx = orig;
    while(1){
        if(ht->used[idx] && ht->table[idx] == value)
            return 1;
        idx = (idx + 1) == ht->table_size ? 0 : (idx + 1);
        if(idx == orig)
            return 0;
    }
}

static u8 hashtable_insert_closed(HT *ht, i64 value){
    if(hashtable_search_closed(ht, value))
        return 0;
    siz idx = ht->func(ht->table_size, value);
    if(ht->used[idx]){
        siz nidx = ht->prober(ht, idx);
        if(nidx == idx)
            return 0;
        ht->table[nidx] = value;
        ht->used[nidx] = 1;
    }
    else{
        ht->used[idx] = 1;
        ht->table[idx] = value;
    }
    return 1;
}

HT* hashtable_create_closed(siz size, HashFunction fun, ProbingTechnique prober){
    HT *ht = (HT *)malloc(sizeof(HT));
    ht->table_size = size;
    ht->table = (i64 *)malloc(sizeof(i64) * size);
    ht->used = (u8 *)malloc(sizeof(u8) * size);
    ht->type = TABLE_CLOSED;
    memset(ht->used, 0, sizeof(u8) * size);
    switch(fun){
        case HF_DIVISION:
            ht->func = hf_division;
            break;
        case HF_FOLDING:
            ht->func = hf_folding;
            break;
        default:
            ht->func = hf_midsq;
    }
    switch(prober){
        case PROBE_LINEAR:
            ht->prober = hp_linear;
            break;
        case PROBE_RANDOM:
            ht->prober = hp_random;
            break;
        case PROBE_QUADRATIC:
            ht->prober = hp_quadratic;
            break;
    }
    ht->insert = hashtable_insert_closed;
    ht->search = hashtable_search_closed;
    ht->free = hashtable_free_closed;
    return ht;
}

u8 hashtable_insert(HT *ht, i64 val){
    return ht->insert(ht, val);
}

u8 hashtable_search(HT *ht, i64 val){
    return ht->search(ht, val);
}

void hashtable_free(HT *ht){
    ht->free(ht);
}

static HT *test_closed_ht;

static u8 test_hashtable_create_closed(){
    test_closed_ht = hashtable_create_closed(HASHTABLE_TEST_ITEM_COUNT, HF_MIDSQUARE, PROBE_LINEAR);
    u8 ret = test_closed_ht ? 1 : 0;
    if(test_closed_ht->table_size != HASHTABLE_TEST_ITEM_COUNT)
        ret &= 0;
    if(test_closed_ht->func != hf_midsq)
        ret &= 0;
    if(test_closed_ht->prober != hp_linear)
        ret &= 0;

    return ret;
}

static u8 test_hashtable_search(HT *ht){
    memset(ht->used, 0, sizeof(u8) * ht->table_size);
    if(hashtable_search(ht, random_at_most(rand())))
        return 0;
    i64 random = random_at_most(5892901);
    ht->table[ht->func(ht->table_size, random)] = random;
    ht->used[ht->func(ht->table_size, random)] = 1;
    if(!hashtable_search(ht, random))
        return 0;
    return 1;
}

static u8 test_hashtable_insert_closed(HT *ht){
    memset(ht->used, 0, sizeof(u8) * ht->table_size);
    i64 random = random_at_most(89892);
    u8 ret = 1;
    if(!hashtable_insert(ht, random))
        ret &= 0;
    if(hashtable_insert(ht, random))
        ret &= 0;
    memset(ht->used, 0, sizeof(u8) * ht->table_size);
    i64 *arr = arr_new(ht->table_size);
    arr_fill_rand(arr, HASHTABLE_TEST_ITEM_COUNT, 901942, SAMPLE_CASE_BEST);
    for(i64 i = 0;i < HASHTABLE_TEST_ITEM_COUNT;i++){
        if(!hashtable_insert(ht, arr[i])){
            ret &= 0;
            break;
        }
    }
    free(arr);
    return ret;
}

void test_hashtable(){
    tst_suite_start("HashTable (Closed)", 3);
    TEST("Create", test_hashtable_create_closed());
    TEST("Search", test_hashtable_search(test_closed_ht));
    TEST("Insert", test_hashtable_insert_closed(test_closed_ht));
    hashtable_free(test_closed_ht);
    tst_suite_end();
}
