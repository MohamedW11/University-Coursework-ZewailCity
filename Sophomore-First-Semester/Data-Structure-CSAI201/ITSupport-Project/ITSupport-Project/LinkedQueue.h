#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <cstddef>

// Linked FIFO queue with head and tail pointers.
// Supports removal of any element matching a predicate (needed for NR cancellation).
template <typename T>
class LinkedQueue {
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

    // Non-copyable (avoids accidental shallow copies)
    LinkedQueue(const LinkedQueue&);
    LinkedQueue& operator=(const LinkedQueue&);

public:
    LinkedQueue() : head(nullptr), tail(nullptr), count(0) {}
    ~LinkedQueue() { clear(); }

    bool isEmpty() const { return head == nullptr; }
    size_t size() const { return count; }

    void enqueue(const T& item) {
        Node* n = new Node(item);
        if (tail) tail->next = n; else head = n;
        tail = n;
        ++count;
    }

    // Removes the front element into 'out'. Returns false if empty.
    bool dequeue(T& out) {
        if (!head) return false;
        Node* n = head;
        out = n->data;
        head = head->next;
        if (!head) tail = nullptr;   // queue became empty
        delete n;
        --count;
        return true;
    }

    // Reads the front element without removing it. Returns false if empty.
    bool peek(T& out) const {
        if (!head) return false;
        out = head->data;
        return true;
    }

    // Removes the FIRST element for which pred(element) is true.
    // Order of the remaining elements is unchanged. Returns true if something was removed.
    template <typename Pred>
    bool removeFirstIf(Pred pred) {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur) {
            if (pred(cur->data)) {
                if (prev) prev->next = cur->next; else head = cur->next;
                if (cur == tail) tail = prev;   // removed the last node
                delete cur;
                --count;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // Visit every element from front to back (useful for interactive display).
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
