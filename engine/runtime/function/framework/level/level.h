#pragma once
#include <vector>
class Object;

class Level {
   public:
    Level(/* args */) {};
    ~Level() {};

   private:
    std::vector<Object> m_objects;
};
