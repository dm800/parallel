#include <Matrix/matrix.h>
#include <cassert>

Matrix Matrix::operator*(const Matrix& other) const {
    assert(this->collumns == other.rows);
    std::vector<std::vector<double>> result(this->rows, std::vector<double>(other.collumns, 0));
    for (unsigned int i = 0; i < this->rows; i++) {
        for (unsigned int j = 0; j < other.collumns; j++) {
            double s = 0;
            for (unsigned int k = 0; k < this->collumns; k++) {
                s += (this)->data[i][k] * other.data[k][j];
            }
            result[i][j] = s;
        }
    }
    return Matrix(result);
}


Matrix Matrix::operator%(const Matrix& other) const {
    assert(this->collumns == other.rows);
    std::vector<std::vector<double>> result(this->rows, std::vector<double>(other.collumns, 0));
    for (unsigned int i = 0; i < this->collumns; i++) {
        for (unsigned int j = 0; j < other.rows; j++) {
            double s = 0;
            for (unsigned int k = 0; k < this->collumns; k++) {
                s += (this)->data[j][k] * other.data[k][i];
            }
            result[j][i] = s;
        }
    }
    return Matrix(result);
}