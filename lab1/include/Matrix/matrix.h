#pragma once
#include <vector>
#include <Generate/randomgen.h>
#include <iosfwd>

class Matrix {
public:
    unsigned int rows;
    unsigned int collumns;

    std::vector<std::vector<double>> data;

    Matrix(unsigned int rows, unsigned int collumns) {
        this->rows = rows;
        this->collumns = collumns;
        std::vector<std::vector<double>> default_data(this->rows, std::vector<double>(this->collumns, 0));
        this->data = default_data;
    }

    Matrix() {
        this->rows = 0;
        this->collumns = 0;
        this->data = {};
    }

    explicit Matrix(const std::vector<std::vector<double>>& data) {
        this->data = data;
        this->rows = data.size();
        if (!data.empty()) {
            this->collumns = data[0].size();
        } else {
            this->collumns = 0;
        }
    }

    std::vector<double>& operator[] (unsigned int index) {
        return data[index];
    }

    Matrix operator* (const Matrix&) const;
    Matrix operator% (const Matrix&) const;
    Matrix operator- (const Matrix&) const;

    Matrix operator* (double alpha) const;

    Matrix operator+ (const Matrix&) const;
    bool operator== (const Matrix&) const;

    Matrix pmulty (Matrix&, unsigned int); // параллельноумножим
    Matrix pmultycol (Matrix&, unsigned int); // параллельноумножим

    void randomize() {
        this->data = randomGen(this->rows, this->collumns);
    }

};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
Matrix operator* (double, const Matrix&);