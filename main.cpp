#include <iostream>

#include "image/Image.hpp"
#include "process/Kernel.hpp"
#include "process/Naive.hpp"
#include <chrono>

int main() {
    Image inFile("../images/boy.ppm");
    Image outFile(inFile.GetWidth(), inFile.GetHeight(), 255);
    Kernel kernel("../kernels/box.k", 3, 3);

    Naive naive(&inFile, &outFile, &kernel);

    int N = 30;
    double totalTime = 0;

    for (int i = 0; i < N; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        naive.Run();
        auto end = std::chrono::high_resolution_clock::now();
        totalTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }

    totalTime = totalTime / (N * 1000000);
    std::cout << "Naive Runtime: " << totalTime << "ms" << std::endl;

    outFile.WriteToFile("../images/test.ppm");

    return 0;
}