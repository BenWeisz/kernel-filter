#include "image/Image.hpp"

Image::Image(const std::string& path) : m_Width(-1), m_Height(-1) {
    int pathLength = path.length();
    // Make sure that the file is ppm
    if (path.substr(pathLength - 4, 4).compare(".ppm")) {
        std::cout << "ERROR: The file is not a ppm file: " << path << std::endl;
        return;
    }

    // Read the data into a char buffer
    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open()) {
        std::cout << "ERROR: The file: " << path << " could not be opened" << std::endl;
        return;
    }

    // Load the file header
    int r = LoadHeader(file);
    if (r == 1) {
        file.close();
        return;
    }

    // Load the file data
    LoadData(file);

    file.close();
}

int Image::LoadHeader(std::ifstream& file) {
    char type[2];
    file.read(type, 2);
    if (type[0] != 'P' || type[1] != '6') {
        std::cout << "ERROR: File must be of P6 type" << std::endl;
        return -1;
    }

    char buffer;

    // Discard the space character
    file.read(&buffer, 1);

    // Read the bytes required for the width
    std::vector<char> widthBuffer;
    file.read(&buffer, 1);
    while ((int)buffer >= '0' && (int)buffer <= '9') {
        widthBuffer.push_back(buffer);
        file.read(&buffer, 1);
    }
    widthBuffer.push_back('\0');

    m_Width = std::stoi(std::string(&widthBuffer[0]));

    // Read the bytes required for the height
    std::vector<char> heightBuffer;
    file.read(&buffer, 1);
    while ((int)buffer >= '0' && (int)buffer <= '9') {
        heightBuffer.push_back(buffer);
        file.read(&buffer, 1);
    }
    heightBuffer.push_back('\0');

    m_Height = std::stoi(std::string(&heightBuffer[0]));

    // Read the bytes required for the quanta
    std::vector<char> quantaBuffer;
    file.read(&buffer, 1);
    while ((int)buffer >= '0' && (int)buffer <= '9') {
        quantaBuffer.push_back(buffer);
        file.read(&buffer, 1);
    }
    quantaBuffer.push_back('\0');

    m_Quanta = std::stoi(std::string(&quantaBuffer[0]));

    // Read the 0x0c character
    file.read(&buffer, 1);

    return 0;
}

void Image::LoadData(std::ifstream& file) {
    char buffer;
    for (int i = 0; i < 3 * m_Width * m_Height; i++) {
        file.read(&buffer, 1);
        float colour = (float)buffer / m_Quanta;
        if (i % 3 == 0)
            m_ChannelR.push_back(colour);
        else if (i % 3 == 1)
            m_ChannelG.push_back(colour);
        else
            m_ChannelB.push_back(colour);
    }
}

void Image::WriteToFile(const std::string& path) const {
    std::ofstream file(path, std::ofstream::binary);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not create file: " << path << std::endl;
        file.close();
        return;
    }

    std::string widthStr = std::to_string(m_Width);
    std::string heightStr = std::to_string(m_Height);
    std::string quantaStr = std::to_string(m_Quanta);
    int headerLength = 7 + widthStr.length() + heightStr.length() + quantaStr.length();

    int dataLength = m_Width * m_Height * 3;
    char* data = new char[headerLength + dataLength];

    int i = 0;

    // Write the header data
    data[i++] = 'P';
    data[i++] = '6';
    data[i++] = ' ';
    for (int j = 0; j < widthStr.length(); j++) {
        data[i++] = widthStr[j];
    }
    data[i++] = ' ';
    for (int j = 0; j < heightStr.length(); j++) {
        data[i++] = heightStr[j];
    }
    data[i++] = ' ';
    for (int j = 0; j < quantaStr.length(); j++) {
        data[i++] = quantaStr[j];
    }
    data[i++] = 0x0a;
    data[i++] = 0x0c;

    // Write the image data
    for (int j = 0; j < m_Width * m_Height; j++) {
        data[i++] = (char)(m_ChannelR[j] * m_Quanta);
        data[i++] = (char)(m_ChannelG[j] * m_Quanta);
        data[i++] = (char)(m_ChannelB[j] * m_Quanta);
    }

    // Write the data to the file
    file.write(data, headerLength + dataLength);

    delete[] data;
    file.close();
}

void Image::SetPixel(const int y, const int x, const float value) {
}
