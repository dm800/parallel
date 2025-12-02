#pragma once
#include <vector>
#include <string>

class Wiseman;
class Fork;

class Manager {
    size_t ticks = 0;
    int spaghetti = 0;
    int number = 1;
    std::vector<Wiseman*> wisemans;
    std::vector<Fork*> forks;
    std::string logs;

    Fork* prev(int);
    Fork* next(int);


public:
    Manager(const std::string&);
    int add_wiseman(Wiseman*);
    void add_fork(Fork*);
    void tick();
    void resolve();
    bool pick_left(int);
    bool pick_right(int);
    bool eat(int);
    void drop(int);
    void log(const std::string&);
    void print_stats();
};
