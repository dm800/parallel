#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <Matrix/matrix.h>

#define SIZE 40000

void get_rows_range(int N, int size, int rank, int& row_start, int& row_end) {
    int base  = N / size;
    int extra = N % size;

    if (rank < extra) {
        row_start = rank * (base + 1);
        row_end   = row_start + (base + 1);
    } else {
        row_start = extra * (base + 1) + (rank - extra) * base;
        row_end   = row_start + base;
    }
}

double dot_omp(const Matrix& v1, const Matrix& v2) {
    if (v1.rows != v2.rows || v1.collumns != v2.collumns) {
        std::cerr << "dot_omp: size mismatch\n";
        std::abort();
    }

    double global_sum = 0.0;

    #pragma omp parallel
    {
        int tid  = omp_get_thread_num();
        int nthr = omp_get_num_threads();

        int row_start = 0, row_end = 0;
        get_rows_range(v1.rows, nthr, tid, row_start, row_end);

        double local_sum = 0.0;
        for (int i = row_start; i < row_end; ++i) {
            for (unsigned int j = 0; j < v1.collumns; ++j) {
                local_sum += v1.data[i][j] * v2.data[i][j];
            }
        }

        #pragma omp atomic
        global_sum += local_sum;
    }

    return global_sum;
}

double norm_omp(const Matrix& v) {
    double sq = dot_omp(v, v);
    return std::sqrt(sq);
}

void matvec_omp(const Matrix& A, const Matrix& x, Matrix& y) {
    if (A.collumns != x.rows || x.collumns != 1 || y.rows != A.rows || y.collumns != 1) {
        std::cerr << "matvec_omp: size mismatch\n";
        std::abort();
    }

    #pragma omp parallel
    {
        int tid  = omp_get_thread_num();
        int nthr = omp_get_num_threads();

        int row_start = 0, row_end = 0;
        get_rows_range(A.rows, nthr, tid, row_start, row_end);

        for (int i = row_start; i < row_end; ++i) {
            double sum = 0.0;
            for (unsigned int j = 0; j < A.collumns; ++j) {
                sum += A.data[i][j] * x.data[j][0];
            }
            y.data[i][0] = sum;
        }
    }
}

int main(int argc, char** argv) {
    const int N        = SIZE;
    const double EPS   = 1e-6;
    const int MAX_ITER = 100000;

    std::cout << "OpenMP threads = " << omp_get_max_threads() << std::endl;

    Matrix A(N, N);

    #pragma omp parallel
    {
        int tid  = omp_get_thread_num();
        int nthr = omp_get_num_threads();

        int row_start = 0, row_end = 0;
        get_rows_range(N, nthr, tid, row_start, row_end);

        for (int i = row_start; i < row_end; ++i) {
            for (int j = 0; j < N; ++j) {
                A.data[i][j] = (i == j ? 2.0 : 1.0);
            }
        }
    }

    Matrix x(N, 1);
    Matrix b(N, 1);
    Matrix r(N, 1);
    Matrix r1(N, 1);
    Matrix z(N, 1);
    Matrix Az(N, 1);
    Matrix Ax(N, 1);

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        b.data[i][0] = N + 1.0;
        x.data[i][0] = (i % 2 == 0 ? 0.0 : -1.0);
    }

    matvec_omp(A, x, Ax);
    r = b - Ax;
    z = r;

    double norm_b  = norm_omp(b);
    double epsilon = norm_omp(r) / norm_b;

    std::cout << "Initial epsilon = " << epsilon << std::endl;

    int    iter    = 0;
    double t_start = omp_get_wtime();

    while (epsilon > EPS && iter < MAX_ITER) {
        matvec_omp(A, z, Az);

        double rr   = dot_omp(r, r);
        double Az_z = dot_omp(Az, z);
        double alpha = rr / Az_z;

        x  = x + alpha * z;
        r1 = r - alpha * Az;

        double rr1 = dot_omp(r1, r1);
        double beta = rr1 / rr;

        z = r1 + beta * z;
        r = r1;

        epsilon = norm_omp(r) / norm_b;
        ++iter;

        std::cout << "iter " << iter
                  << ", epsilon = " << epsilon << std::endl;
    }

    double t_end = omp_get_wtime();

    std::cout << "\nConjugate gradient (OpenMP, lab3) finished\n";
    std::cout << "iterations = " << iter << std::endl;
    std::cout << "epsilon    = " << epsilon << std::endl;
    std::cout << "time       = " << (t_end - t_start) << " s" << std::endl;
    std::cout << "result x:\n" << x << std::endl;

    return 0;
}
