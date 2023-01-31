#include <iostream>

#include "image/Image.hpp"
#include "process/Kernel.hpp"

int main() {
    Image image("../images/boy.ppm");
    image.WriteToFile("../images/out.ppm");

    Image a(2, 2, 255);
    a.m_ChannelR[0] = 1.0;
    a.m_ChannelR[2] = 1.0;
    a.WriteToFile("../images/test.ppm");

    Kernel k("../kernels/box.k", 3, 3);
    for (auto& e : k.GetData()) {
        std::cout << e << " ";
    }

    return 0;
}