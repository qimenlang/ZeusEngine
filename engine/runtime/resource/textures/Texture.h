#pragma once
#include <string>

unsigned int TextureFromFile(const std::string &filename, bool gamma = false);

struct Texture {
    Texture() = default;

    Texture(const Texture &other)
        : id(other.id), type(other.type), path(other.path) {}

    Texture(unsigned int idIn, std::string typeIn) : id(idIn), type(typeIn) {};
    Texture(std::string pathIn, std::string typeIn)
        : type(typeIn), path(pathIn) {
        id = TextureFromFile(path);
    };
    unsigned int id;
    std::string type;
    std::string path;
};
