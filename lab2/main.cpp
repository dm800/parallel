#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <Matrix/matrix.h>

#define SIZE 40000

void get_rows_range(int N, int size, int rank, int& row_start, int& row_end) {
    int base = N / size;
    int extra = N % size;

    if (rank < extra) {
        row_start = rank * (base + 1);
        row_end = row_start + (base + 1);
    } else {
        row_start = extra * (base + 1) + (rank - extra) * base;
        row_end = row_start + base;
    }
}

double dot_parallel(const Matrix& v1, const Matrix& v2, int row_start, int row_end) {
    if (v1.rows != v2.rows || v1.collumns != v2.collumns) {
        std::cerr << "dot_parallel: size mismatch\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    double local = 0.0;
    for (int i = row_start; i < row_end; ++i) {
        for (unsigned int j = 0; j < v1.collumns; ++j) {
            local += v1.data[i][j] * v2.data[i][j];
        }
    }

    double global = 0.0;
    MPI_Allreduce(&local, &global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return global;
}

double norm_parallel(const Matrix& v, int row_start, int row_end) {
    double sq = dot_parallel(v, v, row_start, row_end);
    return std::sqrt(sq);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0, size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = SIZE;
    const double EPS = 1e-6;
    const int MAX_ITER = 100000;

    int row_start = 0, row_end = 0;
    get_rows_range(N, size, rank, row_start, row_end);
    int local_rows = row_end - row_start;

    if (rank == 0) {
        std::cout << "MPI size = " << size << std::endl;
    }
    std::cout << "Rank " << rank << " handles rows ["
              << row_start << ", " << row_end << ")\n";

    Matrix A_local(local_rows, N);
    for (int i = 0; i < local_rows; ++i) {
        int global_row = row_start + i;
        for (int j = 0; j < N; ++j) {
            A_local.data[i][j] = (global_row == j ? 2.0 : 1.0);
        }
    }

    Matrix x(N, 1);
    Matrix b(N, 1);
    Matrix r(N, 1);
    Matrix r1(N, 1);
    Matrix z(N, 1);
    Matrix Az(N, 1);

    for (int i = 0; i < N; ++i) {
        b.data[i][0] = N + 1.0;
        x.data[i][0] = 0;

    }

    std::vector<int> counts(size), displs(size);
    for (int rnk = 0; rnk < size; ++rnk) {
        int rs, re;
        get_rows_range(N, size, rnk, rs, re);
        counts[rnk] = re - rs;
        displs[rnk] = rs;
    }

    Matrix Ax(N, 1);
    std::vector<double> Ax_local_vec(local_rows);
    std::vector<double> Ax_full(N);

    Matrix Ax_local = A_local * x;
    for (int i = 0; i < local_rows; ++i) {
        Ax_local_vec[i] = Ax_local.data[i][0];
    }

    MPI_Allgatherv(Ax_local_vec.data(), local_rows, MPI_DOUBLE,
                   Ax_full.data(), counts.data(), displs.data(),
                   MPI_DOUBLE, MPI_COMM_WORLD);

    for (int i = 0; i < N; ++i) {
        Ax.data[i][0] = Ax_full[i];
    }

    r = b - Ax;
    z = r;

    std::vector<double> Az_local_vec(local_rows);
    std::vector<double> Az_full(N);

    double norm_b = norm_parallel(b, row_start, row_end);
    double epsilon = norm_parallel(r, row_start, row_end) / norm_b;

    if (rank == 0) {
        std::cout << "Initial epsilon = " << epsilon << std::endl;
    }

    int iter = 0;
    double t_start = MPI_Wtime();

    while (epsilon > EPS && iter < MAX_ITER) {
        Matrix Az_local = A_local * z;

        for (int i = 0; i < local_rows; ++i) {
            Az_local_vec[i] = Az_local.data[i][0];
        }

        MPI_Allgatherv(Az_local_vec.data(), local_rows, MPI_DOUBLE,
                       Az_full.data(), counts.data(), displs.data(),
                       MPI_DOUBLE, MPI_COMM_WORLD);

        for (int i = 0; i < N; ++i) {
            Az.data[i][0] = Az_full[i];
        }

        double rr = dot_parallel(r, r, row_start, row_end);
        double Az_z = dot_parallel(Az, z, row_start, row_end);
        double alpha = rr / Az_z;

        x = x + alpha * z;
        r1 = r - alpha * Az;

        double rr1 = dot_parallel(r1, r1, row_start, row_end);
        double beta = rr1 / rr;

        z = r1 + beta * z;
        r = r1;

        epsilon = norm_parallel(r, row_start, row_end) / norm_b;
        ++iter;

        if (rank == 0) {
            std::cout << "iter " << iter
                      << ", epsilon = " << epsilon << std::endl;
        }
    }

    double t_end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "\nConjugate gradient finished" << std::endl;
        std::cout << "iterations = " << iter << std::endl;
        std::cout << "epsilon = " << epsilon << std::endl;
        std::cout << "execution time = " << (t_end - t_start) << " s" << std::endl;
        //std::cout << "result x:\n" << x << std::endl;
    }

    MPI_Finalize();
    return 0;
}

// 1: 15.2826s 20.7013s 20.8069s
// 4: 3.35s 7.86s 7.065s
// 8: 4.41s 2.66s 2.017s