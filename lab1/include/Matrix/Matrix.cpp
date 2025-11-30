#include <cassert>
#include <Matrix/matrix.h>
#include <iostream>

std::ostream& operator << (std::ostream &os, const Matrix &matrix) {
    for (unsigned int i = 0; i < matrix.rows; i++) {
        os << "| ";
        for (unsigned int j = 0; j < matrix.collumns; j++) {
            os << matrix.data[i][j] << "\t";
        }
        os << "|\n";
    }
    return os;
}

bool Matrix::operator==(const Matrix& other) const {
    if (this->collumns != other.collumns or this->rows != other.rows) {
        return false;
    }
    for (unsigned int i = 0; i < this->rows; i++) {
        for (unsigned int j = 0; j < this->collumns; j++) {
            if (this->data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}


Matrix Matrix::operator+(const Matrix & other) const {
    assert(this->rows == other.rows);
    assert(this->collumns == other.collumns);
    std::vector<std::vector<double>> new_data = {};
    for (int i = 0; i < this->rows; i++) {
        std::vector<double> row = {};
        for (int k = 0; k < this->collumns; k++) {
            double number = this->data[i][k] + other.data[i][k];
            row.push_back(number);
        }
        new_data.push_back(row);
    }
    return Matrix(new_data);
}


Matrix Matrix::operator-(const Matrix & other) const {
    assert(this->rows == other.rows);
    assert(this->collumns == other.collumns);
    std::vector<std::vector<double>> new_data = {};
    for (int i = 0; i < this->rows; i++) {
        std::vector<double> row = {};
        for (int k = 0; k < this->collumns; k++) {
            double number = this->data[i][k] - other.data[i][k];
            row.push_back(number);
        }
        new_data.push_back(row);
    }
    return Matrix(new_data);
}


Matrix Matrix::operator* (double alpha) const {
    std::vector<std::vector<double>> ndata(
        this->rows,
        std::vector<double>(this->collumns)
    );

    for (unsigned int i = 0; i < this->rows; ++i) {
        for (unsigned int j = 0; j < this->collumns; ++j) {
            ndata[i][j] = this->data[i][j] * alpha;
        }
    }

    return Matrix(ndata);
}

Matrix operator* (const double alpha, const Matrix& matrix) {
    return matrix * alpha;
}
