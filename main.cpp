#include <iostream>

#include "image/Image.hpp"

int main() {
    Image image("../images/boy.ppm");
    image.WriteToFile("../images/out.ppm");
    return 0;
}