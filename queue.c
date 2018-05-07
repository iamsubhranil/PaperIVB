#include <stdlib.h>

#include "queue.h"
#include "display.h"
#include "test.h"

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
    mint count;
    mints status; // 0->normal, 1 -> full, 2->empty
} CircularQueue;

CircularQueue* queue_new(mint size){
    CircularQueue *queue = (CircularQueue *)malloc(sizeof(CircularQueue));
    queue->count = size;
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
    if(queue->front < queue->rear)
        return queue->rear - queue->front;
    return queue->count - (queue->front - queue->rear);
}

mint queue_size(CircularQueue *queue){
    return queue->count;
}

static inline void check_if_full(CircularQueue *queue){
    if(queue->status == STATUS_EMPTY)
        queue->status = STATUS_OK;
    if(queue->front == queue->rear)
        queue->status = STATUS_FULL;
}

static inline void decr_pointer(CircularQueue *queue, mint *val){
    if(*val == 0)
        *val = queue->count - 1;
    else
        *val = (*val - 1);
}

static inline void incr_pointer(CircularQueue *queue, mint *val){
    *val = (*val + 1) % queue->count;
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
    mint bak = queue->rear;
    queue_insert(queue, value);
    return queue->arr[bak] == value;
}

static mint test_queue_insert_at_front(CircularQueue *queue, mint value){
    mint bak = queue->front;
    decr_pointer(queue, &bak);
    queue_insert_at_front(queue, value);
    return queue->arr[bak] == value;
}

static mint test_queue_delete(CircularQueue *queue){
    mint val = queue->arr[queue->front];
    return queue_delete(queue) == val;
}

static mint test_queue_delete_from_rear(CircularQueue *queue){
    mint val = queue->arr[queue->rear == 0 ? (queue->count - 1) : (queue->rear - 1)];
    return queue_delete_from_rear(queue) == val;
}

static mint test_queue_count(CircularQueue *queue){
    for(mint i = 0;i < 6;i++)
        queue_insert(queue, i);
    return queue_count(queue) == 6;
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
    TEST("Queue Creation", test_queue_new(10));
    TEST("Queue Insertion", test_queue_insert(testQueue, 20));
    TEST("Queue Insertion At Front", test_queue_insert_at_front(testQueue, 10));
    TEST("Queue Deletion", test_queue_delete(testQueue));
    TEST("Queue Deletion From Rear", test_queue_delete_from_rear(testQueue));
    TEST("Queue Counting", test_queue_count(testQueue));
    TEST("Queue Circularity", test_queue_circularity(testQueue));
    queue_free(testQueue);
}
