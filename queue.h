#pragma once

#include "common.h"

#ifdef QUEUE_ENABLE_FAST // Enable fixed-front queue (queue_fast.c)
typedef struct FastCircularQueue* Queue;
#elif defined(QUEUE_ENABLE_LINK) // Enable linked queue (queue_linked.c)
typedef struct LinkedQueue* Queue;
#else
#ifndef QUEUE_ENABLE_CIRCULAR // Default circular queue (queue.c)
#define QUEUE_ENABLE_CIRCULAR
#endif
typedef struct CircularQueue* Queue;
#endif

Queue queue_new(mint size);
void queue_free(Queue queue);

// insert_at_rear
void queue_insert(Queue queue, mint value);
void queue_insert_at_front(Queue queue, mint value);

// delete_from_front
mint queue_delete(Queue queue);
mint queue_delete_from_rear(Queue queue);

mint queue_is_full(Queue queue);
mint queue_is_empty(Queue queue);
mint queue_size(Queue queue); // allocated size of the queue
mint queue_count(Queue queue); // no of elements currently present in the queue
void queue_reset(Queue queue); // resets a queue to the intial state for reuse

void test_queue();

#ifndef QUEUE_TEST_ITEM_COUNT
#define QUEUE_TEST_ITEM_COUNT 10000
#endif

