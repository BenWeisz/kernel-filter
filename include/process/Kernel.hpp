#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

class Kernel {
   private:
    int m_Width;
    int m_Height;
    std::vector<float> m_Data;

   public:
    Kernel(const std::string& path, const int width, const int height);
    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline std::vector<float> GetData() const { return m_Data; }
};