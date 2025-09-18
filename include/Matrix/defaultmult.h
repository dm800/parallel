#pragma once
#include <Matrix/matrix.h>
#include <cassert>

inline Matrix Matrix::operator*(Matrix other) {
    assert(this->collumns == other.rows);
    std::vector<std::vector<double>> result(this->rows, std::vector<double>(other.collumns, 0));
    for (unsigned int i = 0; i < this->rows; i++) {
        for (unsigned int j = 0; j < other.collumns; j++) {
            double s = 0;
            for (unsigned int k = 0; k < this->collumns; k++) {
                s += (*this)[i][k] * other[k][j];
            }
            result[i][j] = s;
        }
    }
    return result;
}


inline Matrix Matrix::operator%(Matrix other) {
    assert(this->collumns == other.rows);
    std::vector<std::vector<double>> result(this->rows, std::vector<double>(other.collumns, 0));
    for (unsigned int i = 0; i < this->collumns; i++) {
        for (unsigned int j = 0; j < other.rows; j++) {
            double s = 0;
            for (unsigned int k = 0; k < this->collumns; k++) {
                s += (*this)[j][k] * other[k][i];
            }
            result[j][i] = s;
        }
    }
    return result;
}