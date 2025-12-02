#pragma once
#include <random>
#include <string>

class Manager;

class Wiseman {
    std::string logs;
    int number;
    int state = 0;
    size_t timer = 0;
    size_t ticks = 0;
    Manager* manager;
    int taken = 0;
    std::random_device rand = std::random_device();
    bool lfork = false;
    bool rfork = false;

    void pick_left();
    void pick_right();
    void drop();
    void eat();
    void think();
    void log(const std::string&);
public:
    Wiseman(Manager*, const std::string&);
    void tick();
    void reset();
};
