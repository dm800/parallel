#include <iostream>
#include <vector>
#include <Matrix/matrix.h>
#include <mpi.h>
#include <cmath>

#define SIZE 8


Matrix x = Matrix();
Matrix A = Matrix();
Matrix b = Matrix({std::vector<double>(SIZE, SIZE + 1)});

Matrix z = Matrix();


Matrix r = Matrix();
Matrix r1 = Matrix();


double alpha = 0;
double beta = 0;

double epsilon = 0;

size_t iter = 0;

double calc_norm(const Matrix& vector) {
    double s = 0;
    for (auto elem : vector.data[0]) {
        s += std::pow(elem, 2);
    }
    return std::sqrt(s);
}


double calc_epsilon() {
    return calc_norm(r) / calc_norm(b);
}

Matrix calc_x() {
    return x + alpha * z;
}


int main(int argc, char** argv) {
    std::vector<std::vector<double>> data = {};
    for (int i  = 0; i < SIZE; i++) {
        std::vector<double> row = std::vector<double>(SIZE, 1);
        row[i] = 2;
        data.push_back(row);
    }
    A = Matrix(data);
    data = {};
    for (int i  = 0; i < SIZE; i++) {
        std::vector<double> row = {1};
        data.push_back(row);
    }
    x = Matrix(data);
    r = b - A * x;
    std::cout << calc_epsilon();
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::cout << "\nrank " << rank;
    while (epsilon > 10e-6) {
        calc_epsilon();
    }
    MPI_Finalize();
    return 0;
}