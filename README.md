# C语言队列实现 (C Queue Implementation)

[![C](https://img.shields.io/badge/C-99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

一个用C语言实现的高效循环队列（Circular Queue）数据结构，支持动态扩容。

## 功能特性

- **循环队列实现** - 基于数组的循环队列，内存利用率高
- **动态扩容** - 队列满时自动扩容（容量翻倍）
- **完整API** - 提供队列常用操作：入队、出队、查看队头/队尾等
- **查找功能** - 支持元素查找和计数
- **内存安全** - 完善的空指针检查和内存管理

## API 接口

### 创建与销毁

| 函数                                                  | 说明                  |
| ----------------------------------------------------- | --------------------- |
| `Queue* queueCreate(void)`                            | 创建队列（默认容量8） |
| `Queue* queueCreateWithCapacity(int initialCapacity)` | 指定初始容量创建队列  |
| `void queueDestroy(Queue* q)`                         | 销毁队列，释放内存    |

### 入队与出队

| 函数                                    | 说明         |
| --------------------------------------- | ------------ |
| `bool queueEnqueue(Queue* q, Data val)` | 入队操作     |
| `bool queueDequeue(Queue* q)`           | 出队操作     |
| `Data queuePeek(Queue* q)`              | 获取队头元素 |
| `Data queuePeekRear(Queue* q)`          | 获取队尾元素 |

### 容量管理

| 函数                          | 说明             |
| ----------------------------- | ---------------- |
| `bool queueIsEmpty(Queue* q)` | 判断队列是否为空 |
| `bool queueIsFull(Queue* q)`  | 判断队列是否已满 |
| `int queueSize(Queue* q)`     | 获取当前元素个数 |
| `int queueCapacity(Queue* q)` | 获取队列容量     |
| `void queueClear(Queue* q)`   | 清空队列         |

### 查找与遍历

| 函数                                     | 说明                 |
| ---------------------------------------- | -------------------- |
| `bool queueContains(Queue* q, Data val)` | 判断是否包含指定元素 |
| `int queueCount(Queue* q, Data val)`     | 统计元素出现次数     |
| `void queuePrint(Queue* q)`              | 打印队列内容         |

## 使用示例

```c
#include <stdio.h>

int main() {
    // 创建队列
    Queue* q = queueCreate();
    
    // 入队
    queueEnqueue(q, 10);
    queueEnqueue(q, 20);
    queueEnqueue(q, 30);
    
    // 查看队头
    printf("队头: %d\n", queuePeek(q));  // 输出: 队头: 10
    
    // 出队
    queueDequeue(q);
    
    // 打印队列
    queuePrint(q);
    
    // 销毁队列
    queueDestroy(q);
    
    return 0;
}
```

## 编译运行

```bash
# 使用 GCC 编译
gcc -o queue 队列.cpp -std=c99

# 运行
./queue
```

## 数据结构

```c
typedef struct {
    Data* data;         // 数据存储区
    int front;          // 队头索引
    int rear;           // 队尾索引
    int size;           // 当前元素个数
    int capacity;       // 队列容量
} Queue;
```

## 算法复杂度

| 操作           | 时间复杂度 | 空间复杂度 |
| -------------- | ---------- | ---------- |
| 入队 (Enqueue) | 均摊 O(1)  | O(1)       |
| 出队 (Dequeue) | O(1)       | O(1)       |
| 查看队头/队尾  | O(1)       | O(1)       |
| 查找元素       | O(n)       | O(1)       |
| 扩容           | O(n)       | O(n)       |

## 依赖

- **C99 标准** - 使用 `<stdbool.h>` 支持布尔类型
- **标准库**:
  - `<stdio.h>` - 标准输入输出
  - `<stdlib.h>` - 内存分配
  - `<stdbool.h>` - 布尔类型支持

[![C99](https://img.shields.io/badge/C99-Standard-orange.svg)](https://en.wikipedia.org/wiki/C99)
[![GCC](https://img.shields.io/badge/GCC-Compatible-brightgreen.svg)](https://gcc.gnu.org/)
[![Clang](https://img.shields.io/badge/Clang-Compatible-blue.svg)](https://clang.llvm.org/)

## 许可证

MIT License

## 作者信息

**谙弆悕博士（Ailan Anjuxi）**

- 邮箱：[anjuxi.ME@outlook.com](mailto:anjuxi.ME@outlook.com)
- SIP电话：[sip:anjuxi@sip.linphone.org](sip:anjuxi@sip.linphone.org)

---

*本项目为C语言数据结构学习与实践作品，欢迎交流探讨。*
