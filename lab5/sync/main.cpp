#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <cstdlib>

using namespace std;

#define ROWS 1000
#define COLS 1000
#define STEPS 100
#define THREADS 4

atomic<int> working(0);

void enter_step() {
    working.fetch_add(1);
}

void wait_all_finished() {
    working.fetch_sub(1);
    while (working.load() != 0) {
        this_thread::yield();
    }
}

class Worker {
public:
    int tid;
    int rows;
    vector<int> local;
    vector<int> next_local;
    vector<int> inbox_top;
    vector<int> inbox_bottom;
    Worker* up;
    Worker* down;
    thread th;

    Worker(int id, int r)
        : tid(id), rows(r),
          local(r * COLS),
          next_local(r * COLS),
          inbox_top(COLS),
          inbox_bottom(COLS),
          up(nullptr),
          down(nullptr) {}

    void start() {
        th = thread(&Worker::run, this);
    }

    void join() {
        th.join();
    }

    void send() {
        for (int j = 0; j < COLS; j++) {
            up->inbox_bottom[j] = local[j];
            down->inbox_top[j] = local[(rows - 1) * COLS + j];
        }
    }

    int evolve(int alive, int n) {
        if (alive) {
            if (n < 2) return 0;
            if (n == 2 || n == 3) return 1;
            return 0;
        }
        return (n == 3) ? 1 : 0;
    }

    void run() {
        for (int step = 0; step < STEPS; step++) {

            enter_step();
            send();
            wait_all_finished();

            enter_step();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < COLS; j++) {

                    int jl = (j - 1 + COLS) % COLS;
                    int jr = (j + 1) % COLS;
                    int n = 0;

                    if (i == 0) {
                        n += inbox_top[jl];
                        n += inbox_top[j];
                        n += inbox_top[jr];
                    } else {
                        n += local[(i - 1) * COLS + jl];
                        n += local[(i - 1) * COLS + j];
                        n += local[(i - 1) * COLS + jr];
                    }

                    n += local[i * COLS + jl];
                    n += local[i * COLS + jr];

                    if (i == rows - 1) {
                        n += inbox_bottom[jl];
                        n += inbox_bottom[j];
                        n += inbox_bottom[jr];
                    } else {
                        n += local[(i + 1) * COLS + jl];
                        n += local[(i + 1) * COLS + j];
                        n += local[(i + 1) * COLS + jr];
                    }

                    next_local[i * COLS + j] =
                        evolve(local[i * COLS + j], n);
                }
            }
            wait_all_finished();

            enter_step();
            local.swap(next_local);
            wait_all_finished();
        }
    }
};

int main() {
    vector<Worker*> workers;

    for (int i = 0; i < THREADS; i++) {
        int rows =
            (i == THREADS - 1)
                ? ROWS - i * (ROWS / THREADS)
                : ROWS / THREADS;
        workers.push_back(new Worker(i, rows));
    }

    for (int i = 0; i < THREADS; i++) {
        workers[i]->up = workers[(i - 1 + THREADS) % THREADS];
        workers[i]->down = workers[(i + 1) % THREADS];
    }

    for (auto w : workers) {
        for (int& x : w->local) {
            x = rand() % 2;
        }
    }

    for (auto w : workers) w->start();
    for (auto w : workers) w->join();

    cout << "Done\n";
    return 0;
}
