#include "Fork.h"

#include <cassert>


Fork::Fork(Manager* man) {
    this->owner = 0;
    this->manager = man;
    this->locked = false;
}

void Fork::lock() {
    assert(locked == false);
    this->locked = true;
}

void Fork::unlock() {
    assert(locked == true);
    this->locked = false;
}

bool Fork::state() {
    return this->locked;
}

int Fork::get_owner() {
    return this->owner;
}

void Fork::set_owner(int wise) {
    assert(locked == false);
    this->owner = wise;
}
