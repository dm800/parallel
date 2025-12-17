#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <atomic>
#define THREADS 4
#define COUNT 100

struct SpinFlag {
    std::atomic<int> f{0};

    void lock() {
        for (;;) {
            int expected = 0;
            if (f.compare_exchange_weak(expected, 1,
                                        std::memory_order_acq_rel,
                                        std::memory_order_relaxed)) {
                return;
            }
            std::this_thread::yield();
        }
    }

    void unlock() {
        f.store(0, std::memory_order_release);
    }
};

struct Node {
    int value;
    std::atomic<Node*> next;

    explicit Node(int v) : value(v), next(nullptr) {}
};

class LinkedList {
public:
    std::atomic<Node*> head{nullptr};
    Node* tail{nullptr};

    bool contains(int value) const {
        Node* cur = head.load(std::memory_order_acquire);
        while (cur) {
            if (cur->value == value) return true;
            cur = cur->next.load(std::memory_order_acquire);
        }
        return false;
    }

    void add_no_check(int value) {
        Node* n = new Node(value);

        Node* h = head.load(std::memory_order_acquire);
        if (!h) {
            head.store(n, std::memory_order_release);
            tail = n;
            return;
        }

        tail->next.store(n, std::memory_order_release);
        tail = n;
    }

    ~LinkedList() {
        Node* cur = head.load(std::memory_order_acquire);
        while (cur) {
            Node* nxt = cur->next.load(std::memory_order_acquire);
            delete cur;
            cur = nxt;
        }
    }
};

SpinFlag read_lock;
SpinFlag write_lock;

void worker(int num_values, LinkedList& list) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 1000);

    for (int i = 0; i < num_values; i++) {
        int v = dis(gen);

        read_lock.lock();
        bool exists = list.contains(v);
        read_lock.unlock();
        if (exists) continue;

        write_lock.lock();
        if (!list.contains(v)) {
            list.add_no_check(v);
        }
        write_lock.unlock();
    }
}

int main() {
    LinkedList list;

    std::vector<std::thread> threads;
    threads.reserve(THREADS);

    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(worker, COUNT, std::ref(list));
    }

    for (auto& t : threads) {
        t.join();
    }

    read_lock.lock();
    Node* cur = list.head.load(std::memory_order_acquire);
    std::cout << "Result list: ";
    while (cur) {
        std::cout << cur->value << " ";
        cur = cur->next.load(std::memory_order_acquire);
    }
    std::cout << "\n";
    read_lock.unlock();

    return 0;
}
