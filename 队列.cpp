#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 队列数据类型
typedef int Data;

// 队列结构体（循环队列实现）
typedef struct {
    Data* data;         // 数据存储区
    int front;          // 队头索引
    int rear;           // 队尾索引
    int size;           // 当前元素个数
    int capacity;       // 队列容量
} Queue;

// ========== 函数声明 ==========

// 创建与销毁
Queue* queueCreate(void);
Queue* queueCreateWithCapacity(int initialCapacity);
void queueDestroy(Queue* q);

// 入队与出队
bool queueEnqueue(Queue* q, Data val);
bool queueDequeue(Queue* q);
Data queuePeek(Queue* q);
Data queuePeekRear(Queue* q);

// 容量管理
bool queueIsEmpty(Queue* q);
bool queueIsFull(Queue* q);
int queueSize(Queue* q);
int queueCapacity(Queue* q);
void queueClear(Queue* q);

// 查找
bool queueContains(Queue* q, Data val);
int queueCount(Queue* q, Data val);

// 遍历
void queuePrint(Queue* q);

// ========== 函数实现 ==========

// 创建队列（默认容量为8）
Queue* queueCreate(void) {
    return queueCreateWithCapacity(8);
}

// 指定初始容量创建队列
Queue* queueCreateWithCapacity(int initialCapacity) {
    if (initialCapacity <= 0) return NULL;
    
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) return NULL;
    
    q->data = (Data*)malloc(sizeof(Data) * initialCapacity);
    if (!q->data) {
        free(q);
        return NULL;
    }
    
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->capacity = initialCapacity;
    return q;
}

// 销毁队列
void queueDestroy(Queue* q) {
    if (!q) return;
    free(q->data);
    free(q);
}

// 扩容
static bool queueGrow(Queue* q) {
    if (!q) return false;
    
    int newCapacity = q->capacity * 2;
    Data* newData = (Data*)malloc(sizeof(Data) * newCapacity);
    if (!newData) return false;
    
    // 将旧数据按顺序复制到新数组
    for (int i = 0; i < q->size; i++) {
        newData[i] = q->data[(q->front + i) % q->capacity];
    }
    
    free(q->data);
    q->data = newData;
    q->front = 0;
    q->rear = q->size - 1;
    q->capacity = newCapacity;
    return true;
}

// 入队
bool queueEnqueue(Queue* q, Data val) {
    if (!q) return false;
    
    if (queueIsFull(q)) {
        if (!queueGrow(q)) return false;
    }
    
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = val;
    q->size++;
    return true;
}

// 出队
bool queueDequeue(Queue* q) {
    if (!q || queueIsEmpty(q)) return false;
    
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return true;
}

// 获取队头元素
Data queuePeek(Queue* q) {
    if (!q || queueIsEmpty(q)) return 0;
    return q->data[q->front];
}

// 获取队尾元素
Data queuePeekRear(Queue* q) {
    if (!q || queueIsEmpty(q)) return 0;
    return q->data[q->rear];
}

// 判断是否为空
bool queueIsEmpty(Queue* q) {
    return !q || q->size == 0;
}

// 判断是否已满（用于固定容量场景）
bool queueIsFull(Queue* q) {
    return q && q->size == q->capacity;
}

// 获取元素个数
int queueSize(Queue* q) {
    return q ? q->size : 0;
}

// 获取容量
int queueCapacity(Queue* q) {
    return q ? q->capacity : 0;
}

// 清空队列
void queueClear(Queue* q) {
    if (!q) return;
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

// 判断是否包含元素
bool queueContains(Queue* q, Data val) {
    if (!q || queueIsEmpty(q)) return false;
    
    for (int i = 0; i < q->size; i++) {
        int idx = (q->front + i) % q->capacity;
        if (q->data[idx] == val) return true;
    }
    return false;
}

// 统计元素出现次数
int queueCount(Queue* q, Data val) {
    if (!q || queueIsEmpty(q)) return 0;
    
    int count = 0;
    for (int i = 0; i < q->size; i++) {
        int idx = (q->front + i) % q->capacity;
        if (q->data[idx] == val) count++;
    }
    return count;
}

// 打印队列
void queuePrint(Queue* q) {
    if (!q) {
        printf("NULL\n");
        return;
    }
    
    printf("[");
    for (int i = 0; i < q->size; i++) {
        int idx = (q->front + i) % q->capacity;
        printf("%d", q->data[idx]);
        if (i < q->size - 1) printf(", ");
    }
    printf("] (size=%d, capacity=%d, front=%d, rear=%d)\n", 
           q->size, q->capacity, q->front, q->rear);
}

// ========== 测试代码 ==========

int main() {
    printf("=== 队列测试 ===\n\n");
    
    // 创建队列
    Queue* q = queueCreate();
    printf("创建队列:\n");
    queuePrint(q);
    
    // 入队操作
    printf("\n入队元素 10, 20, 30, 40, 50:\n");
    queueEnqueue(q, 10);
    queueEnqueue(q, 20);
    queueEnqueue(q, 30);
    queueEnqueue(q, 40);
    queueEnqueue(q, 50);
    queuePrint(q);
    
    // 查看队头和队尾
    printf("\n队头元素: %d\n", queuePeek(q));
    printf("队尾元素: %d\n", queuePeekRear(q));
    
    // 出队操作
    printf("\n出队一次:\n");
    queueDequeue(q);
    queuePrint(q);
    
    printf("\n再入队 60, 70:\n");
    queueEnqueue(q, 60);
    queueEnqueue(q, 70);
    queuePrint(q);
    
    // 查找测试
    printf("\n是否包含30: %s\n", queueContains(q, 30) ? "是" : "否");
    printf("是否包含100: %s\n", queueContains(q, 100) ? "是" : "否");
    
    // 连续出队
    printf("\n连续出队3次:\n");
    queueDequeue(q);
    queueDequeue(q);
    queueDequeue(q);
    queuePrint(q);
    
    // 测试扩容
    printf("\n入队多个元素测试扩容:\n");
    for (int i = 100; i <= 108; i++) {
        queueEnqueue(q, i);
    }
    queuePrint(q);
    
    // 清空队列
    printf("\n清空队列:\n");
    queueClear(q);
    queuePrint(q);
    
    // 重新入队
    printf("\n重新入队 1, 2, 3:\n");
    queueEnqueue(q, 1);
    queueEnqueue(q, 2);
    queueEnqueue(q, 3);
    queuePrint(q);
    
    // 销毁队列
    queueDestroy(q);
    printf("\n队列已销毁\n");
    
    return 0;
}
