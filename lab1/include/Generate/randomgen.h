#pragma once
#include <random>
#include <vector>


// Вся рандом генерация от -10 до 10
std::vector<std::vector<double>> randomGen(unsigned int rows, unsigned int columns) {
    std::vector<std::vector<double>> result(rows, std::vector<double>(columns));
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < columns; j++) {
            result[i][j] = rand() % 41 - 20;
        }
    }
    return result;
}