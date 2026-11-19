#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <cstddef>

// Priority queue implemented as a sorted linked list.
// Comp::operator()(a, b) must return true if 'a' should be served BEFORE 'b'.
// Elements that compare equal keep insertion order (stable).
// insert: O(n)   peek/dequeue: O(1)
template <typename T, typename Comp>
class PriorityQueue {
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    size_t count;
    Comp before;

    PriorityQueue(const PriorityQueue&);
    PriorityQueue& operator=(const PriorityQueue&);

public:
    PriorityQueue() : head(nullptr), count(0) {}
    ~PriorityQueue() { clear(); }

    bool isEmpty() const { return head == nullptr; }
    size_t size() const { return count; }

    void enqueue(const T& item) {
        Node* n = new Node(item);
        // Move past every node that should come before (or ties with) the new item.
        if (!head || before(item, head->data)) {
            n->next = head;
            head = n;
        } else {
            Node* cur = head;
            while (cur->next && !before(item, cur->next->data))
                cur = cur->next;
            n->next = cur->next;
            cur->next = n;
        }
        ++count;
    }

    bool dequeue(T& out) {
        if (!head) return false;
        Node* n = head;
        out = n->data;
        head = head->next;
        delete n;
        --count;
        return true;
    }

    bool peek(T& out) const {
        if (!head) return false;
        out = head->data;
        return true;
    }

    template <typename Func>
    void forEach(Func f) const {
        for (Node* cur = head; cur; cur = cur->next) f(cur->data);
    }

    void clear() {
        T tmp;
        while (dequeue(tmp)) {}
    }
};

#endif
