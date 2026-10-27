# C Queue Implementation

[![C](https://img.shields.io/badge/C-99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A circular queue in C with automatic growth.

## Features

- **Circular queue** — array-backed, good memory use
- **Automatic resize** — doubles capacity when full
- **Common API** — enqueue, dequeue, peek front/rear, and more
- **Lookup** — find elements and count occurrences
- **Memory safety** — null checks and proper cleanup

## API

### Create and destroy

| Function | Description |
| ----------------------------------------------------- | --------------------- |
| `Queue* queueCreate(void)` | Create a queue (default capacity 8) |
| `Queue* queueCreateWithCapacity(int initialCapacity)` | Create a queue with a given capacity |
| `void queueDestroy(Queue* q)` | Destroy the queue and free memory |

### Enqueue and dequeue

| Function | Description |
| --------------------------------------- | ------------ |
| `bool queueEnqueue(Queue* q, Data val)` | Enqueue |
| `bool queueDequeue(Queue* q)` | Dequeue |
| `Data queuePeek(Queue* q)` | Get the front element |
| `Data queuePeekRear(Queue* q)` | Get the rear element |

### Capacity

| Function | Description |
| ----------------------------- | ---------------- |
| `bool queueIsEmpty(Queue* q)` | Check whether the queue is empty |
| `bool queueIsFull(Queue* q)` | Check whether the queue is full |
| `int queueSize(Queue* q)` | Get the current element count |
| `int queueCapacity(Queue* q)` | Get the queue capacity |
| `void queueClear(Queue* q)` | Clear the queue |

### Lookup and printing

| Function | Description |
| ---------------------------------------- | -------------------- |
| `bool queueContains(Queue* q, Data val)` | Check whether an element exists |
| `int queueCount(Queue* q, Data val)` | Count occurrences of an element |
| `void queuePrint(Queue* q)` | Print the queue contents |

## Example

```c
#include <stdio.h>

int main() {
    // Create a queue
    Queue* q = queueCreate();

    // Enqueue
    queueEnqueue(q, 10);
    queueEnqueue(q, 20);
    queueEnqueue(q, 30);

    // Peek at the front
    printf("front: %d\n", queuePeek(q));  // prints: front: 10

    // Dequeue
    queueDequeue(q);

    // Print the queue
    queuePrint(q);

    // Destroy the queue
    queueDestroy(q);

    return 0;
}
```

## Build and Run

```bash
# GCC
gcc -o queue queue.c -std=c99

# Run
./queue
```

Use your actual source filename if it differs from `queue.c`.

## Data Structure

```c
typedef struct {
    Data* data;         // storage
    int front;          // front index
    int rear;           // rear index
    int size;           // current element count
    int capacity;       // capacity
} Queue;
```

## Complexity

| Operation | Time | Space |
| -------------- | ---------- | ---------- |
| Enqueue | amortized O(1) | O(1) |
| Dequeue | O(1) | O(1) |
| Peek front/rear | O(1) | O(1) |
| Lookup | O(n) | O(1) |
| Resize | O(n) | O(n) |

## Dependencies

- **C99** — uses `<stdbool.h>` for booleans
- **Standard library**:
  - `<stdio.h>` — input/output
  - `<stdlib.h>` — memory allocation
  - `<stdbool.h>` — boolean type

[![C99](https://img.shields.io/badge/C99-Standard-orange.svg)](https://en.wikipedia.org/wiki/C99)
[![GCC](https://img.shields.io/badge/GCC-Compatible-brightgreen.svg)](https://gcc.gnu.org/)
[![Clang](https://img.shields.io/badge/Clang-Compatible-blue.svg)](https://clang.llvm.org/)

## License

MIT License

## Author

**Dr. Ailan Anjuxi (谙弆悕)**

- Email: [anjuxi.ME@outlook.com](mailto:anjuxi.ME@outlook.com)
- SIP: [sip:anjuxi@sip.linphone.org](sip:anjuxi@sip.linphone.org)

---

*A small C data-structure exercise. Feedback welcome.*
