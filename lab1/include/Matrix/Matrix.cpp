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
