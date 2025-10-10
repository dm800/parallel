#pragma once
#include <Matrix/matrix.h>
#include <cassert>
#include <thread>


inline void calculate_part(Matrix* A, Matrix* B, unsigned int x1, unsigned int x2, std::vector<std::vector<double>>* result) {
    if (x2 > A->rows) {
        x2 = A->rows;
    }
    for (unsigned int i = x1; i < x2; i++) {
        for (unsigned int j = 0; j < B->collumns; j++) {
            double s = 0;
            for (unsigned int k = 0; k < B->rows; k++) {
                s += (*A)[i][k] * (*B)[k][j];
            }
            result->at(i)[j] = s;
        }
    }
}

inline Matrix Matrix::pmulty(Matrix& other, unsigned int threads) {
    assert(this->collumns == other.rows);
    assert(threads != 0);
    std::vector<std::vector<double>> result(this->rows, std::vector<double>(other.collumns, 0));
    std::vector<std::thread> pool = {};
    if (threads > this->rows) {
        threads = this->rows;
    }
    int step = this->rows / threads;
    for (unsigned int i = 0; i < this->rows; i += step) {
        std::thread t(calculate_part, this, &other, i, i + step, &result);
        pool.push_back(std::move(t));
    }
    for (std::thread& t : pool) {
        t.join();
    }

    return result;
}


inline void calculate_part_col(Matrix* A, Matrix* B, unsigned int x1, unsigned int x2, std::vector<std::vector<double>>* result) {
    if (x2 > A->collumns) {
        x2 = A->collumns;
    }
    for (unsigned int i = x1; i < x2; i++) {
        for (unsigned int j = 0; j < B->rows; j++) {
            double s = 0;
            for (unsigned int k = 0; k < B->rows; k++) {
                s += (*A)[i][k] * (*B)[k][j];
            }
            result->at(j)[i] = s;
        }
    }
}

inline Matrix Matrix::pmultycol(Matrix& other, unsigned int threads) {
    assert(this->collumns == other.rows);
    assert(threads != 0);
    std::vector<std::vector<double>> result(this->rows, std::vector<double>(other.collumns, 0));
    std::vector<std::thread> pool = {};
    if (threads > this->collumns) {
        threads = this->collumns;
    }
    int step = this->collumns / threads;
    for (unsigned int i = 0; i < this->collumns; i += step) {
        std::thread t(calculate_part, this, &other, i, i + step, &result);
        pool.push_back(std::move(t));
    }
    for (std::thread& t : pool) {
        t.join();
    }

    return result;
}
