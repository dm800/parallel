#include <filesystem>
#include <fstream>
#include <Wiseman.h>
#include <random>
#include <Manager.h>
#include <iostream>

Wiseman::Wiseman(Manager* man, const std::string& path) {
    this->number = man->add_wiseman(this);
    this->logs = path;
    std::fstream stream(this->logs, std::ios::ate);
    this->state = 0;
    this->manager = man;
    std::filesystem::remove(path);

}


void Wiseman::tick() {
    this->ticks += 1;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<int> action(1, 3);
    std::uniform_int_distribution<int> duration(1, 4);
    std::uniform_int_distribution<int> fork(1, 2);
    if (this->state == 0) {
        int value = action(gen);
        switch (value) {
            case 1:
                this->timer = duration(gen);
                this->eat();
                break;
            case 2:
                this->timer = duration(gen);
                this->think();
                break;
            case 3:
                this->timer = 5;
                if (fork(gen) == 1) {
                    this->pick_left();
                } else {
                    this->pick_right();
                }
                break;
            case 4:
                this->drop();
                break;
        }
    } else if (state == 1) {
        this->think();
    } else if (state == 2) {
        this->eat();
    } else if (state == 3) {
        this->pick_left();
    } else if (state == 4) {
        this->pick_right();
    }
}

void Wiseman::think() {
    this->timer -= 1;
    if (this->timer == 0) {
        this->state = 0;
    } else {
        this->state = 1;
    }
    this->log("is thinking");
}

void Wiseman::eat() {
    if (!(this->lfork and this->rfork)) {
        this->timer = 1;
        this->state = 1;
        this->think();
        return;
    }
    bool eaten = this->manager->eat(this->number);
    if (!eaten) {
        this->timer = 1;
        this->think();
        return;
    }
    this->timer -= 1;
    if (this->timer == 0) {
        this->state = 0;
    } else {
        this->state = 2;
    }
    this->log("is eating");
}

void Wiseman::pick_left() {
    this->state = 3;
    if (this->lfork == true && this->timer == 5) {
        this->timer = 1;
        this->state = 1;
        this->think();
        return;
    } else if (this->lfork == true && this->timer != 0) {
        this->timer -= 1;
        this->log("resting with left fork");
    } else if (this->lfork == true) {
        this->state = 0;
        this->log("finished resting with left fork");
    }
    bool took = this->manager->pick_left(this->number);
    if (took) {
        this->lfork = true;
        this->log("took left fork");
    } else {
        this->log("couldn't take left fork, now waiting");
    }
}

void Wiseman::pick_right() {
    this->state = 4;
    if (this->rfork == true && this->timer == 5) {
        this->timer = 1;
        this->state = 1;
        this->think();
        return;
    } else if (this->rfork == true && this->timer != 0) {
        this->timer -= 1;
        this->log("resting with right fork");
    } else if (this->rfork == true) {
        this->state = 0;
        this->log("finished resting with right fork");
    }
    bool took = this->manager->pick_right(this->number);
    if (took) {
        this->rfork = true;
        this->log("took right fork");
    } else {
        this->log("couldn't take right fork, now waiting");
    }
}

void Wiseman::drop() {
    this->manager->drop(this->number);
    this->log("dropped both forks");
    this->rfork = false;
    this->lfork = false;
}

void Wiseman::log(const std::string& str) {
    std::ofstream stream(this->logs, std::ios::app);
    stream << this->ticks << ": Wiseman" << this->number << " " << str << std::endl;
    stream.close();
}

void Wiseman::reset() {
    this->log("being resetted");
    this->manager->drop(this->number);
    this->rfork = false;
    this->lfork = false;
    this->state = 0;
    this->timer = 0;
}
