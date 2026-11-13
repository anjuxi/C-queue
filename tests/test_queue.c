#include "queue.h"

#include <stdio.h>

static int g_failed = 0;

/* 一条检查失败不影响后面的检查，最后统一汇总。 */
#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            printf("失败: %s:%d  %s\n", __FILE__, __LINE__, #cond);     \
            g_failed++;                                                 \
        }                                                               \
    } while (0)

static void test_create(void) {
    printf("测试：创建与销毁!\n");

    Queue *q = queueCreate();
    CHECK(q != NULL);
    CHECK(queueIsEmpty(q));
    CHECK(!queueIsFull(q));
    CHECK(queueSize(q) == 0);
    CHECK(queueCapacity(q) == 8);

    Data out = -1;
    CHECK(!queuePeek(q, &out));
    CHECK(out == -1);          /* 空队列不能改写出参 */
    CHECK(!queuePeekRear(q, &out));
    CHECK(out == -1);

    queueDestroy(q);
    queueDestroy(NULL);        /* 传 NULL 不能崩 */

    CHECK(queueCreateWithCapacity(0) == NULL);
    CHECK(queueCreateWithCapacity(-3) == NULL);

    Queue *fixed = queueCreateFixed(4);
    CHECK(fixed != NULL);
    CHECK(queueCapacity(fixed) == 4);
    queueDestroy(fixed);
}

static void test_basic(void) {
    printf("测试：入队、出队与读取!\n");

    Queue *q = queueCreate();
    for (int i = 1; i <= 5; i++) {
        CHECK(queueEnqueue(q, i * 10));
    }
    CHECK(queueSize(q) == 5);

    Data v = 0;
    CHECK(queuePeek(q, &v) && v == 10);
    CHECK(queuePeekRear(q, &v) && v == 50);

    CHECK(queueDequeue(q));
    CHECK(queuePeek(q, &v) && v == 20);
    CHECK(queueSize(q) == 4);

    CHECK(queueEnqueue(q, 60));
    CHECK(queuePeekRear(q, &v) && v == 60);
    queueDestroy(q);

    q = queueCreate();
    CHECK(!queueDequeue(q));
    CHECK(!queueDequeue(NULL));
    CHECK(!queueEnqueue(NULL, 1));
    queueDestroy(q);
}

static void test_wrap(void) {
    printf("测试：下标绕回!\n");

    Queue *q = queueCreateWithCapacity(4);
    queueEnqueue(q, 1);
    queueEnqueue(q, 2);
    queueEnqueue(q, 3);
    queueDequeue(q);
    queueDequeue(q);

    /* 此时 front = 2，入队两个元素会把 rear 绕回下标 0。 */
    CHECK(queueEnqueue(q, 4));
    CHECK(queueEnqueue(q, 5));
    CHECK(queueSize(q) == 3);

    Data v = 0;
    CHECK(queuePeek(q, &v) && v == 3);
    CHECK(queuePeekRear(q, &v) && v == 5);

    int expect[] = {3, 4, 5};
    for (int i = 0; i < 3; i++) {
        CHECK(queuePeek(q, &v) && v == expect[i]);
        CHECK(queueDequeue(q));
    }
    CHECK(queueIsEmpty(q));
    queueDestroy(q);
}

static void test_grow(void) {
    printf("测试：自动扩容!\n");

    Queue *q = queueCreateWithCapacity(2);
    for (int i = 0; i < 100; i++) {
        CHECK(queueEnqueue(q, i));
    }
    CHECK(queueSize(q) == 100);
    CHECK(queueCapacity(q) == 128);
    CHECK(!queueIsFull(q));

    for (int i = 0; i < 100; i++) {
        Data v = -1;
        CHECK(queuePeek(q, &v) && v == i);
        CHECK(queueDequeue(q));
    }
    CHECK(queueIsEmpty(q));
    queueDestroy(q);
}

static void test_fixed(void) {
    printf("测试：固定容量队列!\n");

    Queue *q = queueCreateFixed(3);
    CHECK(queueEnqueue(q, 1));
    CHECK(queueEnqueue(q, 2));
    CHECK(queueEnqueue(q, 3));
    CHECK(queueIsFull(q));
    CHECK(!queueEnqueue(q, 4));    /* 满了只拒绝，不扩容 */
    CHECK(queueSize(q) == 3);

    CHECK(queueDequeue(q));
    CHECK(!queueIsFull(q));
    CHECK(queueEnqueue(q, 4));
    CHECK(!queueShrink(q));        /* 固定容量不自动收缩 */

    int expect[] = {2, 3, 4};
    Data v = 0;
    for (int i = 0; i < 3; i++) {
        CHECK(queuePeek(q, &v) && v == expect[i]);
        CHECK(queueDequeue(q));
    }
    CHECK(queueCapacity(q) == 3);
    queueDestroy(q);
}

static void test_clear(void) {
    printf("测试：清空与复用!\n");

    Queue *q = queueCreateWithCapacity(4);
    for (int i = 1; i <= 4; i++) {
        queueEnqueue(q, i);
    }

    queueClear(q);
    CHECK(queueIsEmpty(q));
    CHECK(queueSize(q) == 0);
    CHECK(queueCapacity(q) == 4);  /* 清空只重置游标，不释放内存 */

    CHECK(queueEnqueue(q, 7));
    Data v = 0;
    CHECK(queuePeek(q, &v) && v == 7);
    CHECK(queuePeekRear(q, &v) && v == 7);

    queueClear(NULL);              /* 传 NULL 不能崩 */
    queueDestroy(q);
}

static void test_lookup(void) {
    printf("测试：查找与计数!\n");

    Queue *q = queueCreate();
    int vals[] = {5, 7, 7, 9};
    for (int i = 0; i < 4; i++) {
        queueEnqueue(q, vals[i]);
    }

    CHECK(queueContains(q, 7));
    CHECK(queueContains(q, 5));
    CHECK(!queueContains(q, 100));
    CHECK(queueCount(q, 7) == 2);
    CHECK(queueCount(q, 5) == 1);
    CHECK(queueCount(q, 100) == 0);

    CHECK(!queueContains(NULL, 5));
    CHECK(queueCount(NULL, 5) == 0);
    queueDestroy(q);
}

static void test_resize(void) {
    printf("测试：预留与收缩!\n");

    Queue *q = queueCreateWithCapacity(4);
    for (int i = 0; i < 10; i++) {
        CHECK(queueEnqueue(q, i));
    }
    CHECK(queueCapacity(q) == 16);

    CHECK(queueShrink(q));
    CHECK(queueCapacity(q) == 10);  /* 正好收缩到元素个数 */

    for (int i = 0; i < 10; i++) {
        Data v = -1;
        CHECK(queuePeek(q, &v) && v == i);
        CHECK(queueDequeue(q));
    }

    for (int i = 0; i < 10; i++) {
        CHECK(queueEnqueue(q, i));
    }
    for (int i = 0; i < 6; i++) {
        CHECK(queueDequeue(q));
    }
    CHECK(queueShrink(q));
    CHECK(queueCapacity(q) == 8);   /* 低于下限就停在 8 */

    int expect[] = {6, 7, 8, 9};
    Data v = 0;
    for (int i = 0; i < 4; i++) {
        CHECK(queuePeek(q, &v) && v == expect[i]);
        CHECK(queueDequeue(q));
    }

    CHECK(queueReserve(q, 100));
    CHECK(queueCapacity(q) == 100);
    CHECK(queueReserve(q, 10));     /* 已经够大，不缩小 */
    CHECK(queueCapacity(q) == 100);
    CHECK(!queueReserve(q, 0));
    CHECK(!queueReserve(NULL, 10));
    queueDestroy(q);
}

static unsigned int g_seed = 20260101u;

/* 固定种子的线性同余随机数，保证每次跑出来的操作序列一样。 */
static unsigned int next_rand(void) {
    g_seed = g_seed * 1103515245u + 12345u;
    return (g_seed >> 16) & 0x7fffu;
}

static void test_stress(void) {
    printf("测试：随机操作对照!\n");

    enum { MAX_LEN = 256, ROUNDS = 100000 };
    int ref[MAX_LEN];
    int ref_len = 0;

    Queue *q = queueCreateWithCapacity(1);
    CHECK(q != NULL);
    if (!q) return;

    for (int step = 0; step < ROUNDS; step++) {
        /* 偏向入队，但长度超过上限时强制出队，防止内存无限涨。 */
        bool do_enqueue = (ref_len == 0) ||
                          (ref_len < MAX_LEN && next_rand() % 3 != 0);

        if (do_enqueue) {
            Data val = (Data)(next_rand() % 1000);
            CHECK(queueEnqueue(q, val));
            ref[ref_len++] = val;
        } else {
            CHECK(queueDequeue(q));
            for (int i = 1; i < ref_len; i++) {
                ref[i - 1] = ref[i];
            }
            ref_len--;
        }

        CHECK(queueSize(q) == ref_len);
        if (ref_len > 0) {
            Data v = -1;
            CHECK(queuePeek(q, &v) && v == ref[0]);
            CHECK(queuePeekRear(q, &v) && v == ref[ref_len - 1]);
        }
    }

    for (int i = 0; i < ref_len; i++) {
        Data v = -1;
        CHECK(queuePeek(q, &v) && v == ref[i]);
        CHECK(queueDequeue(q));
    }
    CHECK(queueIsEmpty(q));
    queueDestroy(q);
}

int main(void) {
    test_create();
    test_basic();
    test_wrap();
    test_grow();
    test_fixed();
    test_clear();
    test_lookup();
    test_resize();
    test_stress();

    if (g_failed == 0) {
        printf("全部测试通过!\n");
        return 0;
    }

    printf("有 %d 项检查失败!\n", g_failed);
    return 1;
}
