#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <Fork.h>
#include <Wiseman.h>
#include <Manager.h>
#include <filesystem>
#include <ranges>

Manager::Manager(const std::string& path) {
    this->wisemans = std::vector<Wiseman*>();
    this->forks = std::vector<Fork*>();
    this->logs = path;
    std::filesystem::remove(path);
}

Fork* Manager::prev(int wise) {
    return this->forks[wise - 1];
}

Fork *Manager::next(int wise) {
    return this->forks[wise % 5];
}


bool Manager::eat(int wise) {
    bool permission = prev(wise)->get_owner() == wise and next(wise)->get_owner() == wise;
    if (permission) {
        spaghetti += 1;
        this->log("Wiseman" + std::to_string(wise) + " ate");
    } else {
        this->log("Wiseman" + std::to_string(wise) + " failed to eat");
    }
    this->log(std::to_string(wise - 1) + " fork owner " + std::to_string(prev(wise)->get_owner()));
    this->log(std::to_string(wise % 5) + " fork owner " + std::to_string(next(wise)->get_owner()));
    return permission;
}

void Manager::log(const std::string& str) {
    std::ofstream stream(this->logs, std::ios::app);
    if (!stream.is_open()) {
        raise(1);
    }
    stream << this->ticks << ": Manager " << str << std::endl;
    stream.close();
}


void Manager::tick() {
    std::shuffle(wisemans.begin(), wisemans.end(), std::mt19937(std::random_device()()));
    for (auto man : this->wisemans) {
        man->tick();
    }
    this->resolve();
}

void Manager::drop(int wise) {
    if (this->prev(wise)->get_owner() == wise) {
        this->prev(wise)->unlock();
        this->prev(wise)->set_owner(0);
    } else if (this->next(wise)->get_owner() == wise) {
        this->next(wise)->unlock();
        this->next(wise)->set_owner(0);
    }
}

int Manager::add_wiseman(Wiseman* wise) {
    wisemans.push_back(wise);
    this->number += 1;
    return this->number - 1;
}

bool Manager::pick_left(int wise) {
    if (this->prev(wise)->state()) {
        return false;
    }
    this->prev(wise)->set_owner(wise);
    this->prev(wise)->lock();
    return true;
}

bool Manager::pick_right(int wise) {
    if (next(wise)->state()) {
        return false;
    }
    this->next(wise)->set_owner(wise);
    this->next(wise)->lock();
    return true;
}

void Manager::add_fork(Fork* fork) {
    forks.push_back(fork);
}

void Manager::print_stats() {
    std::cout << this->spaghetti << std::endl;
    int co = 0;
    for (auto elem : forks) {
        co += elem->get_owner();
    }
    if (co == 15) {
        std::cout << "MAYBE FOUND" << std::endl;
        for (auto elem : forks) {
            std::cout << elem->get_owner() << elem->state() << " ";
        }
        std::cout << std::endl;
    }
}

void Manager::resolve() {
    std::vector<bool> wisemans_bool = std::vector<bool>(5, false);
    for (auto fork : forks) {
        if (fork->get_owner() != 0) {
            wisemans_bool[fork->get_owner() - 1] = true;
        } else {
            break;
        }
    }
    if (std::ranges::all_of(wisemans_bool, [](bool x) {return x == true; })) {
        wisemans[0]->reset();
        this->log("fixed deadlock");
    }
}
