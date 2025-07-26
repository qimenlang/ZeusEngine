#pragma once
#include <string>
unsigned int TextureFromFile(const char *path, const std::string &directory,
                             bool gamma = false);

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};
