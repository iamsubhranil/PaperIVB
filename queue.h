#pragma once

#include "common.h"

typedef struct CircularQueue* Queue;

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
