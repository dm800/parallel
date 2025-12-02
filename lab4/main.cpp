#include <filesystem>
#include <iostream>
#include <Manager.h>
#include <Fork.h>
#include <Wiseman.h>


void init(Manager& manager) {
    for (int k = 0; k < 5; k++) {
        new Wiseman(&manager, "logs/wiseman" + std::to_string(k + 1) + ".txt");
    }
    for (int i = 0; i < 5; i++) {
        auto* fork = new Fork(&manager);
        manager.add_fork(fork);
    }
}

int main() {

    Manager manager = Manager("logs/Manager.txt");
    init(manager);
    for (int i = 0; i < 10000; i++) {
        manager.tick();
    }
    return 0;
}