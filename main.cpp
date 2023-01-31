#include <iostream>

#include "image/Image.hpp"
#include "process/Kernel.hpp"
#include "process/Naive.hpp"
#include "process/Parallel.hpp"
#include <chrono>

int main() {
    Image inFile("../images/boy.ppm");
    Image outFileNaive(inFile.GetWidth(), inFile.GetHeight(), 255);
    Kernel kernel("../kernels/box.k", 3, 3);

    Naive naive(&inFile, &outFileNaive, &kernel);

    int N = 30;

    // Naive Method Test
    double totalTimeNaive = 0;
    for (int i = 0; i < N; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        naive.Run();
        auto end = std::chrono::high_resolution_clock::now();
        totalTimeNaive += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    totalTimeNaive = totalTimeNaive / (N * 1000000);
    std::cout << "Naive Runtime: " << totalTimeNaive << "ms" << std::endl;

    outFileNaive.WriteToFile("../images/outputNaive.ppm");

    // Parallel Method Test
    Image outFileParallel(inFile.GetWidth(), inFile.GetHeight(), 255);
    Parallel parallel(&inFile, &outFileParallel, &kernel);

    double totalTimeParallel = 0;
    for (int i = 0; i < N; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        parallel.Run();
        auto end = std::chrono::high_resolution_clock::now();
        totalTimeParallel += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    totalTimeParallel = totalTimeParallel / (N * 1000000);
    std::cout << "Parallel Runtime: " << totalTimeParallel << "ms" << std::endl;

    outFileParallel.WriteToFile("../images/outputParallel.ppm");

    return 0;
}