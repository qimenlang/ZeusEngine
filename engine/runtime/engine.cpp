#include <iostream>
#include "engine.h"
namespace zeus{
    engine::engine() {};
    engine::~engine(){};
    void engine::run(){
        while (true)
        {
            std::cout<<"engine runing..."<<std::endl;
        }
    }
}