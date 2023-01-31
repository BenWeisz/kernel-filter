#pragma once

#include <iostream>
#include <vector>

#include "process/Process.hpp"

class Naive : public Process {
   public:
    Naive(Image* inFile, Image* outFile, Kernel* Kernel) : Process(inFile, outFile, Kernel) {}
    void Run() override {
        int imageWidth = m_InFile->GetWidth();
        int imageHeight = m_InFile->GetHeight();
        int kernelWidth = m_Kernel->GetWidth();
        int kernelHeight = m_Kernel->GetHeight();

        for (int y = 0; y < imageHeight; y++) {
            for (int x = 0; x < imageWidth; x++) {
                Convolve(x, y, imageWidth, imageHeight, kernelWidth, kernelHeight);
            }
        }
    }

   private:
    void Convolve(const int cx, const int cy, const int imageWidth, const int imageHeight, const int kernelWidth, const int kernelHeight) {
        int halfw = (kernelWidth - 1) / 2;
        int halfh = (kernelHeight - 1) / 2;

        std::vector<float> kernelData = m_Kernel->GetData();

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
                    pval = m_InFile->m_ChannelR[ppos];
                    totalR += kval * pval;

                    // G Channel
                    pval = m_InFile->m_ChannelG[ppos];
                    totalG += kval * pval;

                    // B Channel
                    pval = m_InFile->m_ChannelB[ppos];
                    totalB += kval * pval;
                }
            }
        }

        ppos = (cy * imageWidth) + cx;

        m_OutFile->m_ChannelR[ppos] = totalR;
        m_OutFile->m_ChannelG[ppos] = totalG;
        m_OutFile->m_ChannelB[ppos] = totalB;
    }
};