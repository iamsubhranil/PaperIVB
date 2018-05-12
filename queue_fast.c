#include <string.h>

#include "queue.h"
#include "utils.h"
#include "test.h"

#ifdef QUEUE_ENABLE_FAST

typedef struct FastCircularQueue{
    i64 *arr;
    i64 rear; // we don't need a front, as front will always be at 0
    i64 size;
} FastCircularQueue;

FastCircularQueue* queue_new(i64 size){
    FastCircularQueue *q = (FastCircularQueue *)malloc(sizeof(FastCircularQueue));
    q->arr = (i64 *)malloc(sizeof(i64) * size);
    q->size = size;
    q->rear = -1;
    return q;
}

void queue_free(FastCircularQueue *q){
    free(q->arr);
    free(q);
}

void queue_insert(FastCircularQueue *q, i64 val){
    q->rear < q->size && (q->arr[++q->rear] = val);
}

static inline i64 shift_right(FastCircularQueue *q){
    memcpy(q->arr+1, q->arr, sizeof(i64)*(q->rear + 1));
    return 1;
}

void queue_insert_at_front(FastCircularQueue *q, i64 val){
    (q->rear < q->size) && (shift_right(q), q->arr[0] = val, q->rear++);
}

i64 queue_delete(FastCircularQueue *q){
    i64 bak = 0;
    q->rear >= 0 && (bak = q->arr[0], 
            memcpy(q->arr, q->arr + 1, sizeof(i64)*q->rear),
            q->rear--);
    return bak;
}

i64 queue_delete_from_rear(FastCircularQueue *q){
    return q->rear >= 0 ? q->arr[q->rear--] : 0;
}

u8 queue_is_full(FastCircularQueue *q){
    return q->size == q->rear+1;
}

u8 queue_is_empty(FastCircularQueue *q){
    return q->rear == -1;
}

i64 queue_size(FastCircularQueue *q){
    return q->size;
}

i64 queue_count(FastCircularQueue *q){
    return q->rear + 1;
}

void queue_reset(FastCircularQueue *q){
    q->rear = -1;
}


static FastCircularQueue *testQueue;

static i64 test_queue_new(i64 size){
    FastCircularQueue *queue = queue_new(size);
    if(!queue)
        return 0;
    testQueue = queue;
    return 1;
}

static i64 test_queue_insert(FastCircularQueue *queue, i64 value){
    i64 bak = random_at_most(value);
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert(queue, bak);
    }
    return queue->arr[queue->size - 1] == bak;
}

static i64 test_queue_insert_at_front(FastCircularQueue *queue, i64 value){
    queue_reset(queue);
    i64 bak = random_at_most(value);
    queue_insert(queue, bak);
    i64 rearval = queue->arr[queue->rear];
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert_at_front(queue, bak);
    }
    return queue->arr[0] == bak && queue->arr[queue->rear] == rearval;
}

static i64 test_queue_delete(FastCircularQueue *queue){
    queue_reset(queue);
    i64 bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert_at_front(queue, bak);
    }
    return queue_delete(queue) == bak;
}

static i64 test_queue_delete_from_rear(FastCircularQueue *queue){
    queue_reset(queue);
    i64 bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert(queue, bak);
    }
    return queue_delete_from_rear(queue) == bak;
}

static i64 test_queue_count(FastCircularQueue *queue){
    queue_reset(queue);
    i64 testSize = random_at_most(queue->size);
    for(i64 i = 0;i < testSize;i++)
        queue_insert(queue, random_at_most(46734));
    if(queue_count(queue) != testSize)
        return 0;
    while(!queue_is_full(queue))
        queue_insert(queue, random_at_most(988945));
    if(queue_count(queue) != queue->size)
        return 0;
    while(!queue_is_empty(queue))
        queue_delete(queue);
    if(queue_count(queue) != 0)
        return 0;
    return 1;
}

void test_queue(){
    tst_suite_start("Queue", 6);
    TEST("Creation", test_queue_new(QUEUE_TEST_ITEM_COUNT));
    TEST("Insertion", test_queue_insert(testQueue, 4823));
    TEST("Insertion At Front", test_queue_insert_at_front(testQueue, 239943));
    TEST("Deletion", test_queue_delete(testQueue));
    TEST("Deletion From Rear", test_queue_delete_from_rear(testQueue));
    TEST("Counting", test_queue_count(testQueue));
    queue_free(testQueue);
    tst_suite_end();
}

#endif
