#include "queue.h"

#include <stdio.h>

/* 演示程序：基本操作、绕回、扩容和固定容量队列。 */
int main(void) {
    printf("=== 循环队列演示 ===\n");

    Queue *q = queueCreate();
    if (!q) {
        printf("创建队列失败!\n");
        return 1;
    }

    printf("默认容量: %d!\n", queueCapacity(q));
    queuePrint(q);

    printf("\n入队 10, 20, 30, 40, 50!\n");
    for (int i = 1; i <= 5; i++) {
        queueEnqueue(q, i * 10);
    }
    queuePrint(q);

    Data head = 0;
    Data tail = 0;
    if (queuePeek(q, &head) && queuePeekRear(q, &tail)) {
        printf("队头 %d，队尾 %d!\n", head, tail);
    }

    printf("\n出队一次!\n");
    queueDequeue(q);
    queuePrint(q);

    printf("\n再入队 60, 70，rear 在这里绕回数组开头!\n");
    queueEnqueue(q, 60);
    queueEnqueue(q, 70);
    queuePrint(q);

    printf("\n再入队 4 个元素，容量不够自动翻倍!\n");
    for (int i = 80; i <= 110; i += 10) {
        queueEnqueue(q, i);
    }
    queuePrint(q);

    printf("\n查找 30 和 100!\n");
    printf("包含 30: %s!\n", queueContains(q, 30) ? "是" : "否");
    printf("包含 100: %s!\n", queueContains(q, 100) ? "是" : "否");

    printf("\n出队 6 次后收缩内存!\n");
    for (int i = 0; i < 6; i++) {
        queueDequeue(q);
    }
    printf("收缩前容量: %d!\n", queueCapacity(q));
    queueShrink(q);
    printf("收缩后容量: %d!\n", queueCapacity(q));
    queuePrint(q);

    queueDestroy(q);
    printf("\n自动扩容队列已销毁!\n");

    /* 固定容量队列：满了以后入队直接失败，不扩容。 */
    Queue *fixed = queueCreateFixed(3);
    printf("\n固定容量队列（容量 3）!\n");
    queueEnqueue(fixed, 1);
    queueEnqueue(fixed, 2);
    queueEnqueue(fixed, 3);
    printf("已满: %s!\n", queueIsFull(fixed) ? "是" : "否");
    printf("再入队 4: %s!\n", queueEnqueue(fixed, 4) ? "成功" : "失败");
    queuePrint(fixed);
    queueDestroy(fixed);
    printf("固定容量队列已销毁!\n");

    return 0;
}
