#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

/* 元素类型。换类型时把 QUEUE_DATA_FORMAT 一起改掉。 */
typedef int Data;

/* queuePrint 用的格式串，要和 Data 对上。 */
#define QUEUE_DATA_FORMAT "%d"

/* front 指向队头，rear 指向下一个空位，size 单独计数。
   auto_grow 为 true 时满了翻倍，为 false 时容量固定。 */
typedef struct {
    Data *data;
    int front;
    int rear;
    int size;
    int capacity;
    bool auto_grow;
} Queue;

/* 创建与销毁 */
Queue *queueCreate(void);                      /* 默认容量 8，自动扩容 */
Queue *queueCreateWithCapacity(int initial_capacity);
Queue *queueCreateFixed(int capacity);         /* 固定容量，满了入队失败 */
void queueDestroy(Queue *q);

/* 入队与出队，成功返回 true */
bool queueEnqueue(Queue *q, Data val);
bool queueDequeue(Queue *q);

/* 读队头、队尾。成功返回 true，数据写进 out；
   空队列或 out 为空返回 false，out 不动。 */
bool queuePeek(const Queue *q, Data *out);
bool queuePeekRear(const Queue *q, Data *out);

/* 状态查询，NULL 一律按空队列处理 */
bool queueIsEmpty(const Queue *q);
bool queueIsFull(const Queue *q);
int queueSize(const Queue *q);
int queueCapacity(const Queue *q);

/* 容量管理 */
bool queueReserve(Queue *q, int capacity);     /* 扩容到至少 capacity，不缩小 */
bool queueShrink(Queue *q);                    /* 收缩到 size（不低于 8），仅自动扩容队列 */
void queueClear(Queue *q);

/* 查找与打印 */
bool queueContains(const Queue *q, Data val);
int queueCount(const Queue *q, Data val);
void queuePrint(const Queue *q);

#endif /* QUEUE_H */
