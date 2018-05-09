#include "queue.h"
#include "utils.h"
#include "test.h"

#ifdef QUEUE_ENABLE_LINK

typedef struct LinkedNode{
    mint val;
    struct LinkedNode *next;
} LinkedNode;

typedef struct LinkedQueue{
    LinkedNode *rear;
    LinkedNode *front;
    mint size;
    mint count;
} LinkedQueue;

static LinkedNode* linkednode_get(mint val){
    LinkedNode *node = (LinkedNode *)malloc(sizeof(LinkedNode));
    node->next = NULL;
    node->val = val;
    return node;
}

LinkedQueue* queue_new(mint size){
    LinkedQueue *q = (LinkedQueue *)malloc(sizeof(LinkedQueue));
    q->count = 0;
    q->size = size;
    q->front = q->rear = NULL;
    return q;
}

static void queue_free_nodes(LinkedNode *node){
    while(node){
        LinkedNode *bak = node->next;
        free(node);
        node = bak;
    }
}

void queue_free(LinkedQueue *q){
    queue_free_nodes(q->front);
    free(q);
}

void queue_insert(LinkedQueue *q, mint val){
    if(q->count == q->size)
        return;
    if(q->rear == NULL){
        q->rear = q->front = linkednode_get(val);
    }
    else{
        LinkedNode *node = linkednode_get(val);
        q->rear->next = node;
        q->rear = node;
    }
    q->count++;
}

void queue_insert_at_front(LinkedQueue *q, mint val){
    if(q->count == q->size)
        return;
    if(q->rear == NULL){
        q->rear = q->front = linkednode_get(val);
    }
    else{
        LinkedNode *node = linkednode_get(val);
        node->next = q->front;
        q->front = node;
    }
    q->count++;
}

mint queue_delete(LinkedQueue *q){
    if(q->count == 0)
        return 0;
    mint bak = q->front->val;
    LinkedNode *n = q->front;
    q->front = q->front->next;
    if(!q->front)
        q->rear = NULL;
    free(n);
    q->count--;
    return bak;
}

mint queue_delete_from_rear(LinkedQueue *q){
    if(q->count == 0)
        return 0;
    LinkedNode *bak = q->rear;
    mint bval = bak->val;
    LinkedNode *parent = q->front;
    while(parent->next != bak) parent = parent->next;
    parent->next = NULL;
    free(bak);
    q->count--;
    return bval;
}

mint queue_is_full(LinkedQueue *q){
    return q->count == q->size;
}

mint queue_is_empty(LinkedQueue *q){
    return q->count == 0;
}

mint queue_size(LinkedQueue *q){
    return q->size;
}

mint queue_count(LinkedQueue *q){
    return q->count;
}

void queue_reset(LinkedQueue *q){
    queue_free_nodes(q->front);
    q->front = q->rear = NULL;
    q->count = 0;
}


static LinkedQueue *testQueue;

static mint test_queue_new(mint size){
    LinkedQueue *queue = queue_new(size);
    if(!queue)
        return 0;
    testQueue = queue;
    return 1;
}

static mint test_queue_insert(LinkedQueue *queue, mint value){
    mint bak = random_at_most(value);
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert(queue, bak);
    }
    return queue->rear->val == bak;
}

static mint test_queue_insert_at_front(LinkedQueue *queue, mint value){
    queue_reset(queue);
    mint bak = random_at_most(value);
    queue_insert(queue, bak);
    mint rearval = queue->rear->val;
    while(!queue_is_full(queue)){
        bak = random_at_most(value);
        queue_insert_at_front(queue, bak);
    }
    return queue->front->val == bak && queue->rear->val == rearval;
}

static mint test_queue_delete(LinkedQueue *queue){
    queue_reset(queue);
    mint bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert_at_front(queue, bak);
    }
    return queue_delete(queue) == bak;
}

static mint test_queue_delete_from_rear(LinkedQueue *queue){
    queue_reset(queue);
    mint bak = random_at_most(73723);
    queue_insert(queue, bak);
    while(!queue_is_full(queue)){
        bak = random_at_most(477285);
        queue_insert(queue, bak);
    }
    return queue_delete_from_rear(queue) == bak;
}

static mint test_queue_count(LinkedQueue *queue){
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
