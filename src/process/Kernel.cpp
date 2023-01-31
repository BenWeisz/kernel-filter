#include "process/Kernel.hpp"

Kernel::Kernel(const std::string& path, const int width, const int height) : m_Width(width), m_Height(height) {
    std::ifstream file(path, std::ios_base::binary);

    // Ensure the file is open
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open file: " << path << std::endl;
        file.close();
        return;
    }

    // Read and normalize the data
    char buffer;
    int i = 0;
    while (i < m_Width * m_Height) {
        file.read(&buffer, 1);
        m_Data.push_back((float)buffer / 255.0);
        i++;
    }

    file.close();
}