#include <stdlib.h>

#include "queue.h"
#include "display.h"
#include "test.h"
#include "utils.h"

/* Architechture
 * =============
 * Here the circular queue is implemented by means of
 * two usual pointers - FRONT and REAR. Only difference
 * is that here REAR is always one up the last element,
 * i. e., REAR always stays in the position where
 * the new element is going to be inserted. However,
 * FRONT stays in the position where the first item
 * to be deleted presently is. It looks a bit like
 * the following :
 *
 * 34   74   84   48    49   91
 * FRONT                           REAR
 *  
 * While inserting at rear, the regular insertion,
 * at first the item is placed in the REAR, and then
 * only it is updated. Also the empty and full flags
 * are updated, if required.
 *
 * While inserting at the front, at first FRONT
 * is incremented, and then the new item is placed
 * at new FRONT. The same checking is performed here
 * too.
 *
 * While deleting from the front, the regular deletion,
 * a copy of the item of the front is created, then
 * FRONT is decremented, and a check is performed
 * to set or reset the status flag as required, and
 * then the copy is returned.
 *
 * While deleting from the rear, at first the REAR is
 * decremented, and the value of the item present
 * at new REAR is returned after performing usual
 * checks. This is consistent with out concept of
 * speculative increment of the REAR too. Since an
 * item is deleted from the rear, any new item is
 * going to be inserted at one index before where
 * it was going to be previously. And again, that
 * is pointed by REAR.
 *
 * Quite surprisingly, both the queue full and queue
 * empty is handled by one condition
 * 
 * if(FRONT == REAR)
 *
 * after each operation. It may seem absurd at first,
 * but it is infact quite natural, as it is performed
 * _after_ every opertion.
 *
 */

typedef enum{
    STATUS_OK = 0,
    STATUS_FULL = 1,
    STATUS_EMPTY = 2
} QueueStatus;

typedef struct CircularQueue{
    mint *arr;
    mint front;
    mint rear;
    mint size;
    mints status; // 0->normal, 1 -> full, 2->empty
} CircularQueue;

CircularQueue* queue_new(mint size){
    CircularQueue *queue = (CircularQueue *)malloc(sizeof(CircularQueue));
    queue->size = size;
    queue->arr = (mint *)malloc(sizeof(mint) * size);
    queue->rear = 0;
    queue->front = -1;
    queue->status = STATUS_EMPTY;
    return queue;
}

mint queue_is_full(CircularQueue *queue){
    return queue->status == STATUS_FULL;
}

mint queue_is_empty(CircularQueue *queue){
    return queue->status == STATUS_EMPTY;
}

mint queue_count(CircularQueue *queue){
    if(queue_is_full(queue))
        return queue->size;
    if(queue_is_empty(queue))
        return 0;
    if(queue->front < queue->rear)
        return queue->rear - queue->front;
    return queue->size - (queue->front - queue->rear);
}

mint queue_size(CircularQueue *queue){
    return queue->size;
}

void queue_reset(CircularQueue *queue){
    queue->status = STATUS_EMPTY;
    queue->front = -1;
    queue->rear = 0;
}

static inline void check_if_full(CircularQueue *queue){
    if(queue->status == STATUS_EMPTY)
        queue->status = STATUS_OK;
    if(queue->front == queue->rear)
        queue->status = STATUS_FULL;
}

static inline void decr_pointer(CircularQueue *queue, mint *val){
    if(*val == 0 || *val == -1)
        *val = queue->size - 1;
    else
        *val = (*val - 1);
}

static inline void incr_pointer(CircularQueue *queue, mint *val){
    *val = (*val + 1) % queue->size;
}

void queue_insert(CircularQueue* queue, mint value){
    if(queue_is_full(queue))
        return;
    queue->arr[queue->rear] = value;
    incr_pointer(queue, &queue->rear);
    if(queue->front == -1)
        queue->front = 0;
    check_if_full(queue);
}

void queue_insert_at_front(CircularQueue *queue, mint value){
    if(queue_is_full(queue))
        return;
    decr_pointer(queue, &queue->front);
    queue->arr[queue->front] = value;
    check_if_full(queue);
}

static inline void check_if_empty(CircularQueue *queue){
    if(queue->status == STATUS_FULL)
        queue->status = STATUS_OK;
    if(queue->front == queue->rear)
        queue->status = STATUS_EMPTY;
}

mint queue_delete(CircularQueue *queue){
    if(queue_is_empty(queue))
        return 0;
    mint bak = queue->arr[queue->front];
    incr_pointer(queue, &queue->front);
    check_if_empty(queue);
    return bak;
}

mint queue_delete_from_rear(CircularQueue *queue){
    if(queue_is_empty(queue))
        return 0;
    decr_pointer(queue, &queue->rear);
    check_if_empty(queue);
    return queue->arr[queue->rear];
}

void queue_free(CircularQueue *queue){
    free(queue->arr);
    free(queue);
}

static CircularQueue *testQueue;

static mint test_queue_new(mint size){
    CircularQueue *queue = queue_new(size);
    if(!queue)
        return 0;
    testQueue = queue;
    return 1;
}

static mint test_queue_insert(CircularQueue *queue, mint value){
    mint bak = random_at_most(value);
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert(queue, bak);
    }
    mint p = queue->rear;
    decr_pointer(queue, &p);
    return queue->arr[p] == bak;
}

static mint test_queue_insert_at_front(CircularQueue *queue, mint value){
    queue_reset(queue);
    mint bak = random_at_most(value);
    mint p = queue->rear;
    decr_pointer(queue, &p);
    mint rear = queue->rear;
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert_at_front(queue, bak);
    }
    return queue->arr[queue->front] == bak && queue->rear == rear;
}

static mint test_queue_delete(CircularQueue *queue){
    queue_reset(queue);
    mint bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert_at_front(queue, bak);
    }
    return queue_delete(queue) == bak;
}

static mint test_queue_delete_from_rear(CircularQueue *queue){
    queue_reset(queue);
    mint bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert(queue, bak);
    }
    return queue_delete_from_rear(queue) == bak;
}

static mint test_queue_count(CircularQueue *queue){
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

static mint test_queue_circularity(CircularQueue *queue){
    while(!queue_is_full(queue))
        queue_insert_at_front(queue, 34);
    mint last;
    while(!queue_is_empty(queue))
        queue_delete_from_rear(queue);
    while(!queue_is_full(queue))
        queue_insert(queue, 34);
    while(!queue_is_empty(queue))
        last = queue_delete(queue);
    return last == 34;
}

void test_queue(){
    TEST("Queue Creation", test_queue_new(100000));
    TEST("Queue Insertion", test_queue_insert(testQueue, 4823));
    TEST("Queue Insertion At Front", test_queue_insert_at_front(testQueue, 239943));
    TEST("Queue Deletion", test_queue_delete(testQueue));
    TEST("Queue Deletion From Rear", test_queue_delete_from_rear(testQueue));
    TEST("Queue Counting", test_queue_count(testQueue));
    TEST("Queue Circularity", test_queue_circularity(testQueue));
    queue_free(testQueue);
}
