#define QUEUE_MAX_SIZE 30  // set as needed

typedef struct {
    int data[QUEUE_MAX_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int size;
} Queue;


int queue_size(Queue *queue) {
    if (queue->tail >= queue->head)
        return queue->tail - queue->head;
    else
        return queue->size - (queue->head - queue->tail);
}

int queue_init(Queue *queue, unsigned int size) {
    if (size > QUEUE_MAX_SIZE) {
        return 1;  // return 1 if size too large, else 0
    }
    queue->head = 0;
    queue->tail = 0;
    queue->size = size;
    return 0;
}

int queue_is_full(Queue *queue) {
    return ((queue->tail + 1) % queue->size) == queue->head;
}

int queue_is_empty(Queue *queue) {
    return queue->tail == queue->head;
}

int queue_enqueue(Queue *queue, int item) {
    if (!queue_is_full(queue)) {
        queue->data[queue->tail++] = item;
        queue->tail %= queue->size;
        return 1;
    } else {
        return 0;
    }
}

int queue_dequeue(Queue *queue, int *item) {
    if (!queue_is_empty(queue)) {
        *item = queue->data[queue->head++];
        queue->head %= queue->size;
        return 1;
    } else {
        return 0;
    }
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
