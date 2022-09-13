#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>

char signs[] = "@%$0Oo?/[{|;:*^_,. ";

std::vector<char> readBMP(const std::string& file)
{
    int div = sizeof(signs) / sizeof(char);
    static constexpr size_t HEADER_SIZE = 54;
    std::ifstream bmp(file, std::ios::binary);

    std::array<char, HEADER_SIZE> header;
    bmp.read(header.data(), header.size());

    int fileSize = *reinterpret_cast<uint32_t*>(&header[2]);
    int dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
    int width = *reinterpret_cast<uint32_t*>(&header[18]);
    int height = *reinterpret_cast<uint32_t*>(&header[22]);
    int depth = *reinterpret_cast<uint16_t*>(&header[28]);

    std::cout << "fileSize: " << fileSize << std::endl;
    std::cout << "dataOffset: " << dataOffset << std::endl;
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    std::cout << "depth: " << depth << "-bit" << std::endl;

    std::vector<char> img(dataOffset - HEADER_SIZE);
    bmp.read(img.data(), img.size());

    auto dataSize = ((width * 3 + 3) & (~3)) * height;
    img.resize(dataSize);
    bmp.read(img.data(), img.size());

    char temp = 0;

    for (int i = 54; i < dataSize - 4; i += 3)
    {
        //std::cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i + 1] & 0xff) << " B: " << int(img[i + 2] & 0xff) << std::endl;
        if (abs((img[i] + img[i + 1] + img[i + 2])) / 3 > 0)
        {
            std::cout << signs[(img[i] + img[i + 1] + img[i + 2])/3 / div] << " ";
        }
        else
            std::cout << "  ";
        if (i % (width*3) == 0)
            std::cout << std::endl;
    }

    return img;
}

int main()
{
    //std::cout << fileSize("test.bmp");
    readBMP("test.bmp");
}