#pragma once

#include <string>

#include "image/Image.hpp"
#include "process/Kernel.hpp"

class Process {
   protected:
    Image* m_InFile;
    Image* m_OutFile;
    Kernel* m_Kernel;

   public:
    Process(Image* inFile, Image* outFile, Kernel* kernel) : m_InFile(inFile), m_OutFile(outFile), m_Kernel(kernel) {}
    virtual void Run() = 0;
};