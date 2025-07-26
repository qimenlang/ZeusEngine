#pragma once
#include <string>
unsigned int TextureFromFile(const std::string &filename, bool gamma = false);

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};
