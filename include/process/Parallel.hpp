#pragma once

#include <iostream>
#include <vector>
#include <thread>

#include "process/Process.hpp"

class Parallel : public Process {
   public:
    Parallel(Image* inFile, Image* outFile, Kernel* Kernel) : Process(inFile, outFile, Kernel) {}
    void Run() override {
        int imageWidth = m_InFile->GetWidth();
        int imageHeight = m_InFile->GetHeight();
        int kernelWidth = m_Kernel->GetWidth();
        int kernelHeight = m_Kernel->GetHeight();

        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0 || imageHeight < 16)
            numThreads = 1;

        numThreads = 4;

        int chunkSize = imageHeight / numThreads;
        int chunkStart;
        int chunkEnd;

        std::thread* threads = new std::thread[numThreads];
        for (int i = 0; i < numThreads; i++) {
            chunkStart = i * numThreads;
            chunkEnd = (i + 1) * numThreads;
            if (i + 1 == numThreads)
                chunkEnd = imageHeight;

            threads[i] = std::thread(Task, chunkStart, chunkEnd, m_InFile, m_OutFile, m_Kernel);
        }

        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
        }

        delete[] threads;
    }

   private:
    static void Task(const int start, const int end, Image* inFile, Image* outFile, Kernel* kernel) {
        int imageWidth = inFile->GetWidth();
        int imageHeight = inFile->GetHeight();
        int kernelWidth = kernel->GetWidth();
        int kernelHeight = kernel->GetHeight();
        for (int y = start; y < end; y++) {
            for (int x = 0; x < imageWidth; x++) {
                Convolve(x, y, inFile, outFile, kernel);
            }
        }
    }
    static void Convolve(const int cx, const int cy, Image* inFile, Image* outFile, Kernel* kernel) {
        int imageWidth = inFile->GetWidth();
        int imageHeight = inFile->GetHeight();
        int kernelWidth = kernel->GetWidth();
        int kernelHeight = kernel->GetHeight();

        int halfw = (kernelWidth - 1) / 2;
        int halfh = (kernelHeight - 1) / 2;

        std::vector<float> kernelData = kernel->GetData();

        float totalR = 0.0;
        float totalG = 0.0;
        float totalB = 0.0;

        int px;
        int py;
        int ppos;
        float pval;

        int yy;
        int xx;
        int kpos;
        float kval;
        for (int y = -halfh; y <= halfh; y++) {
            for (int x = -halfw; x <= halfw; x++) {
                // Pixel coordinates in image
                px = cx + x;
                py = cy + y;

                // Ensure that we are reading pixel values from a valid image coordinate
                if (px >= 0 && px < imageWidth && py >= 0 && py < imageHeight) {
                    // Convert the indexes into valid kernel indices
                    yy = y + halfh;
                    xx = x + halfw;
                    kpos = (yy * kernelWidth) + xx;

                    // Get the kernel value
                    kval = kernelData[kpos];

                    ppos = (py * imageWidth) + px;

                    // R Channel
                    pval = inFile->m_ChannelR[ppos];
                    totalR += kval * pval;

                    // G Channel
                    pval = inFile->m_ChannelG[ppos];
                    totalG += kval * pval;

                    // B Channel
                    pval = inFile->m_ChannelB[ppos];
                    totalB += kval * pval;
                }
            }
        }

        ppos = (cy * imageWidth) + cx;

        outFile->m_ChannelR[ppos] = totalR;
        outFile->m_ChannelG[ppos] = totalG;
        outFile->m_ChannelB[ppos] = totalB;
    }
};