#pragma once
#include <vector>
#include <Generate/randomgen.h>

class Matrix {
public:
    unsigned int rows;
    unsigned int collumns;

    std::vector<std::vector<double>> data;

    Matrix(unsigned int rows, unsigned int collumns) {
        this->rows = rows;
        this->collumns = collumns;
        std::vector<std::vector<double>> default_data(this->collumns, std::vector<double>(this->rows, 0));
        this->data = default_data;
    }

    Matrix(std::vector<std::vector<double>> data) {
        this->data = data;
        this->rows = data.size();
        this->collumns = data[0].size();
    }

    std::vector<double>& operator[] (unsigned int index) {
        return data[index];
    }

    Matrix operator* (Matrix);
    Matrix operator% (Matrix);
    bool operator== (Matrix);

    Matrix pmulty (Matrix&, unsigned int); // параллельноумножим
    Matrix pmultycol (Matrix&, unsigned int); // параллельноумножим

    void randomize() {
        this->data = randomGen(this->rows, this->collumns);
    }

};

inline std::ostream& operator << (std::ostream &os, const Matrix &matrix) {
    for (unsigned int i = 0; i < matrix.rows; i++) {
        os << "| ";
        for (unsigned int j = 0; j < matrix.collumns; j++) {
            os << matrix.data[i][j] << "\t";
        }
        os << "|\n";
    }
    return os;
}

inline bool Matrix::operator==(Matrix other) {
    if (this->collumns != other.collumns or this->rows != other.rows) {
        return false;
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->collumns; j++) {
            if (this->data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}
