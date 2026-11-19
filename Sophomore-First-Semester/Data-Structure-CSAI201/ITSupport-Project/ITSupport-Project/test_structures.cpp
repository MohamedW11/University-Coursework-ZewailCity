#include <iostream>
#include <cassert>
#include <vector>
#include "LinkedQueue.h"
#include "PriorityQueue.h"

using namespace std;

// ---- Stand-in for a UR ticket, just for testing the priority rule ----
struct TestUR {
    int rid, qt, pr;
    TestUR() : rid(0), qt(0), pr(0) {}
    TestUR(int r, int q, int p) : rid(r), qt(q), pr(p) {}
};

// Higher PR first, then earlier QT, then smaller RID
struct URBefore {
    bool operator()(const TestUR& a, const TestUR& b) const {
        if (a.pr != b.pr) return a.pr > b.pr;
        if (a.qt != b.qt) return a.qt < b.qt;
        return a.rid < b.rid;
    }
};

static int passed = 0;
#define CHECK(cond) do { if (!(cond)) { cout << "FAIL line " << __LINE__ << ": " #cond "\n"; return 1; } ++passed; } while (0)

int testQueueBasics() {
    LinkedQueue<int> q;
    int x;
    CHECK(q.isEmpty());
    CHECK(!q.dequeue(x));            // dequeue on empty
    CHECK(!q.peek(x));
    q.enqueue(1); q.enqueue(2); q.enqueue(3);
    CHECK(q.size() == 3);
    CHECK(q.peek(x) && x == 1);
    CHECK(q.dequeue(x) && x == 1);
    CHECK(q.dequeue(x) && x == 2);
    CHECK(q.dequeue(x) && x == 3);
    CHECK(q.isEmpty());
    q.enqueue(9);                    // reuse after becoming empty (tail reset check)
    CHECK(q.dequeue(x) && x == 9);
    return 0;
}

int testQueueRemove() {
    int x;
    // remove head
    { LinkedQueue<int> q; for (int i = 1; i <= 4; i++) q.enqueue(i);
      CHECK(q.removeFirstIf([](int v){ return v == 1; }));
      CHECK(q.size() == 3 && q.dequeue(x) && x == 2); }
    // remove middle
    { LinkedQueue<int> q; for (int i = 1; i <= 4; i++) q.enqueue(i);
      CHECK(q.removeFirstIf([](int v){ return v == 2; }));
      CHECK(q.dequeue(x) && x == 1);
      CHECK(q.dequeue(x) && x == 3);
      CHECK(q.dequeue(x) && x == 4); }
    // remove tail, then enqueue again (tail pointer must be fixed)
    { LinkedQueue<int> q; for (int i = 1; i <= 3; i++) q.enqueue(i);
      CHECK(q.removeFirstIf([](int v){ return v == 3; }));
      q.enqueue(7);
      CHECK(q.dequeue(x) && x == 1);
      CHECK(q.dequeue(x) && x == 2);
      CHECK(q.dequeue(x) && x == 7);
      CHECK(q.isEmpty()); }
    // remove only element, then reuse
    { LinkedQueue<int> q; q.enqueue(5);
      CHECK(q.removeFirstIf([](int v){ return v == 5; }));
      CHECK(q.isEmpty());
      q.enqueue(6);
      CHECK(q.dequeue(x) && x == 6); }
    // remove not found
    { LinkedQueue<int> q; q.enqueue(1);
      CHECK(!q.removeFirstIf([](int v){ return v == 99; }));
      CHECK(q.size() == 1); }
    return 0;
}

int testPriorityQueue() {
    PriorityQueue<TestUR, URBefore> pq;
    TestUR t;
    CHECK(pq.isEmpty());
    CHECK(!pq.dequeue(t));

    pq.enqueue(TestUR(10, 5, 3));   // low PR
    pq.enqueue(TestUR(11, 2, 9));   // highest PR
    pq.enqueue(TestUR(12, 1, 9));   // same PR, earlier QT -> before 11
    pq.enqueue(TestUR(9,  1, 9));   // same PR and QT, smaller RID -> first
    pq.enqueue(TestUR(13, 5, 3));   // ties fully with 10 except RID

    int expected[] = {9, 12, 11, 10, 13};
    for (int i = 0; i < 5; i++) {
        CHECK(pq.dequeue(t));
        CHECK(t.rid == expected[i]);
    }
    CHECK(pq.isEmpty());
    return 0;
}

int main() {
    if (testQueueBasics())  return 1;
    if (testQueueRemove())  return 1;
    if (testPriorityQueue()) return 1;
    cout << "All tests passed (" << passed << " checks).\n";
    return 0;
}
