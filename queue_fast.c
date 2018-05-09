#include <string.h>

#include "queue.h"
#include "utils.h"
#include "test.h"

#ifdef QUEUE_ENABLE_FAST

typedef struct FastCircularQueue{
    mint *arr;
    mint rear; // we don't need a front, as front will always be at 0
    mint size;
} FastCircularQueue;

FastCircularQueue* queue_new(mint size){
    FastCircularQueue *q = (FastCircularQueue *)malloc(sizeof(FastCircularQueue));
    q->arr = (mint *)malloc(sizeof(mint) * size);
    q->size = size;
    q->rear = -1;
    return q;
}

void queue_free(FastCircularQueue *q){
    free(q->arr);
    free(q);
}

void queue_insert(FastCircularQueue *q, mint val){
    q->rear < q->size && (q->arr[++q->rear] = val);
}

static inline mint shift_right(FastCircularQueue *q){
    memcpy(q->arr+1, q->arr, sizeof(mint)*(q->rear + 1));
    return 1;
}

void queue_insert_at_front(FastCircularQueue *q, mint val){
    (q->rear < q->size) && (shift_right(q), q->arr[0] = val, q->rear++);
}

mint queue_delete(FastCircularQueue *q){
    mint bak = 0;
    q->rear >= 0 && (bak = q->arr[0], 
            memcpy(q->arr, q->arr + 1, sizeof(mint)*q->rear),
            q->rear--);
    return bak;
}

mint queue_delete_from_rear(FastCircularQueue *q){
    return q->rear >= 0 ? q->arr[q->rear--] : 0;
}

mint queue_is_full(FastCircularQueue *q){
    return q->size == q->rear+1;
}

mint queue_is_empty(FastCircularQueue *q){
    return q->rear == -1;
}

mint queue_size(FastCircularQueue *q){
    return q->size;
}

mint queue_count(FastCircularQueue *q){
    return q->rear + 1;
}

void queue_reset(FastCircularQueue *q){
    q->rear = -1;
}


static FastCircularQueue *testQueue;

static mint test_queue_new(mint size){
    FastCircularQueue *queue = queue_new(size);
    if(!queue)
        return 0;
    testQueue = queue;
    return 1;
}

static mint test_queue_insert(FastCircularQueue *queue, mint value){
    mint bak = random_at_most(value);
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert(queue, bak);
    }
    return queue->arr[queue->size - 1] == bak;
}

static mint test_queue_insert_at_front(FastCircularQueue *queue, mint value){
    queue_reset(queue);
    mint bak = random_at_most(value);
    queue_insert(queue, bak);
    mint rearval = queue->arr[queue->rear];
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert_at_front(queue, bak);
    }
    return queue->arr[0] == bak && queue->arr[queue->rear] == rearval;
}

static mint test_queue_delete(FastCircularQueue *queue){
    queue_reset(queue);
    mint bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert_at_front(queue, bak);
    }
    return queue_delete(queue) == bak;
}

static mint test_queue_delete_from_rear(FastCircularQueue *queue){
    queue_reset(queue);
    mint bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert(queue, bak);
    }
    return queue_delete_from_rear(queue) == bak;
}

static mint test_queue_count(FastCircularQueue *queue){
    queue_reset(queue);
    mint testSize = random_at_most(queue->size);
    for(mint i = 0;i < testSize;i++)
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
    TEST("Queue Creation", test_queue_new(QUEUE_TEST_ITEM_COUNT));
    TEST("Queue Insertion", test_queue_insert(testQueue, 4823));
    TEST("Queue Insertion At Front", test_queue_insert_at_front(testQueue, 239943));
    TEST("Queue Deletion", test_queue_delete(testQueue));
    TEST("Queue Deletion From Rear", test_queue_delete_from_rear(testQueue));
    TEST("Queue Counting", test_queue_count(testQueue));
    queue_free(testQueue);
}

#endif
