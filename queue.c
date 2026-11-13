#include "queue.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_MIN_CAPACITY 8

/* 把从队头算起的第 offset 个元素映射到数组下标。
   调用方保证 0 <= offset < capacity，所以减一次就够，不用取模。 */
static int queueIndex(const Queue *q, int offset) {
    int idx = q->front + offset;
    return idx >= q->capacity ? idx - q->capacity : idx;
}

/* 把已有元素按顺序搬到新数组并把 front 归零。
   new_capacity 不能小于 size，否则调用方应先检查。 */
static bool queueResize(Queue *q, int new_capacity) {
    if (!q || new_capacity < q->size) return false;

    Data *new_data = (Data *)malloc(sizeof(Data) * (size_t)new_capacity);
    if (!new_data) return false;

    for (int i = 0; i < q->size; i++) {
        new_data[i] = q->data[queueIndex(q, i)];
    }

    free(q->data);
    q->data = new_data;
    q->front = 0;
    q->rear = q->size;
    q->capacity = new_capacity;
    return true;
}

/* 容量翻倍。返回 false 表示分配失败，或者再翻就超过 int 上限了。 */
static bool queueGrow(Queue *q) {
    if (!q || q->capacity > INT_MAX / 2) return false;
    return queueResize(q, q->capacity * 2);
}

/* 创建队列的公共实现，auto_grow 决定满了以后扩容还是拒绝入队。 */
static Queue *queueCreateInternal(int initial_capacity, bool auto_grow) {
    if (initial_capacity <= 0) return NULL;

    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q) return NULL;

    q->data = (Data *)malloc(sizeof(Data) * (size_t)initial_capacity);
    if (!q->data) {
        free(q);
        return NULL;
    }

    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity = initial_capacity;
    q->auto_grow = auto_grow;
    return q;
}

Queue *queueCreate(void) {
    return queueCreateInternal(QUEUE_MIN_CAPACITY, true);
}

Queue *queueCreateWithCapacity(int initial_capacity) {
    return queueCreateInternal(initial_capacity, true);
}

Queue *queueCreateFixed(int capacity) {
    return queueCreateInternal(capacity, false);
}

void queueDestroy(Queue *q) {
    if (!q) return;
    free(q->data);
    free(q);
}

bool queueEnqueue(Queue *q, Data val) {
    if (!q) return false;

    if (q->size == q->capacity) {
        /* 固定容量队列到这里只能拒绝；自动扩容队列先腾地方。 */
        if (!q->auto_grow || !queueGrow(q)) return false;
    }

    q->data[q->rear] = val;
    if (++q->rear == q->capacity) q->rear = 0;
    q->size++;
    return true;
}

bool queueDequeue(Queue *q) {
    if (!q || q->size == 0) return false;

    if (++q->front == q->capacity) q->front = 0;
    q->size--;
    return true;
}

bool queuePeek(const Queue *q, Data *out) {
    if (!q || !out || q->size == 0) return false;
    *out = q->data[q->front];
    return true;
}

bool queuePeekRear(const Queue *q, Data *out) {
    if (!q || !out || q->size == 0) return false;

    /* rear 指向下一个空位，所以队尾在它前一个位置，注意绕回。 */
    int idx = q->rear == 0 ? q->capacity - 1 : q->rear - 1;
    *out = q->data[idx];
    return true;
}

bool queueIsEmpty(const Queue *q) {
    return !q || q->size == 0;
}

bool queueIsFull(const Queue *q) {
    return q && q->size == q->capacity;
}

int queueSize(const Queue *q) {
    return q ? q->size : 0;
}

int queueCapacity(const Queue *q) {
    return q ? q->capacity : 0;
}

bool queueReserve(Queue *q, int capacity) {
    if (!q || capacity <= 0) return false;
    if (capacity <= q->capacity) return true;
    return queueResize(q, capacity);
}

bool queueShrink(Queue *q) {
    if (!q) return false;

    /* 固定容量队列的容量是调用方定的，不替它做主。 */
    if (!q->auto_grow) return false;

    int target = q->size > QUEUE_MIN_CAPACITY ? q->size : QUEUE_MIN_CAPACITY;
    if (target >= q->capacity) return true;
    return queueResize(q, target);
}

void queueClear(Queue *q) {
    if (!q) return;

    /* 只重置游标，内存留着给下一次入队用。 */
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

bool queueContains(const Queue *q, Data val) {
    if (!q || q->size == 0) return false;

    for (int i = 0; i < q->size; i++) {
        if (q->data[queueIndex(q, i)] == val) return true;
    }
    return false;
}

int queueCount(const Queue *q, Data val) {
    if (!q || q->size == 0) return 0;

    int count = 0;
    for (int i = 0; i < q->size; i++) {
        if (q->data[queueIndex(q, i)] == val) count++;
    }
    return count;
}

void queuePrint(const Queue *q) {
    if (!q) {
        printf("NULL\n");
        return;
    }

    printf("[");
    for (int i = 0; i < q->size; i++) {
        if (i > 0) printf(", ");
        printf(QUEUE_DATA_FORMAT, q->data[queueIndex(q, i)]);
    }
    printf("] size=%d capacity=%d\n", q->size, q->capacity);
}
