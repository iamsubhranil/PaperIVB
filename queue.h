#pragma once

#include "common.h"

#ifdef QUEUE_ENABLE_FAST // Enable fixed-front queue (queue_fast.c)
typedef struct FastCircularQueue *Queue;
#elif defined(QUEUE_ENABLE_LINK) // Enable linked queue (queue_linked.c)
typedef struct LinkedQueue *Queue;
#else
#ifndef QUEUE_ENABLE_CIRCULAR // Default circular queue (queue.c)
#define QUEUE_ENABLE_CIRCULAR
#endif
typedef struct CircularQueue *Queue;
#endif

Queue queue_new(i64 size);
void  queue_free(Queue queue);

// insert_at_rear
void queue_insert(Queue queue, i64 value);
void queue_insert_at_front(Queue queue, i64 value);

// delete_from_front
i64 queue_delete(Queue queue);
i64 queue_delete_from_rear(Queue queue);

u8   queue_is_full(Queue queue);
u8   queue_is_empty(Queue queue);
i64  queue_size(Queue queue);  // allocated size of the queue
i64  queue_count(Queue queue); // no of elements currently present in the queue
void queue_reset(Queue queue); // resets a queue to the intial state for reuse

void test_queue();
