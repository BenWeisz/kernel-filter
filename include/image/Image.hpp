#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Simple image class to read ppm files
class Image {
   private:
    int m_Width;
    int m_Height;
    int m_Quanta;
    int LoadHeader(std::ifstream& file);
    void LoadData(std::ifstream& file);

   public:
    Image(const std::string& path);
    Image(const int width, const int height, const int quanta);
    void WriteToFile(const std::string& path) const;
    std::vector<float> m_ChannelR;
    std::vector<float> m_ChannelG;
    std::vector<float> m_ChannelB;
};