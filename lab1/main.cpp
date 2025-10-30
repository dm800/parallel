#include <vector>
#include <iostream>
#include <Matrix/matrix.h>
#include <Matrix/defaultmult.h>
#include <Matrix/parallelmult.h>

#define NUMBER_OF_THREADS 8

int main() {

    srand(time(nullptr));
    Matrix matrix1(1000, 1000);
    Matrix matrix2(1000, 1000);
    matrix1.randomize();
    matrix2.randomize();
    std::chrono::steady_clock::time_point start;
    long long duration;


    start = std::chrono::steady_clock::now();

    auto result = matrix1.pmulty(matrix2, NUMBER_OF_THREADS);

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

    std::cout << "Took " << duration << "ms with " << NUMBER_OF_THREADS << " threads" << std::endl;

    start = std::chrono::steady_clock::now();

    auto result2 = matrix1 * matrix2;

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

    std::cout << "Took " << duration << "ms without threads" << std::endl;

    std::cout << "Checking if they are equal" << std::endl;
    std::cout << (result == result2) << std::endl;

    return 0;
}
