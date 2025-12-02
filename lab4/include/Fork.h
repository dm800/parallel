#pragma once

class Manager;

class Fork {
    int owner;
    Manager* manager;
    bool locked;

public:
    Fork(Manager*);
    bool state();
    void lock();
    void unlock();
    int get_owner();
    void set_owner(int);
};
