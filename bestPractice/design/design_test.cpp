#include <iostream>

#include "facade/facade.h"
#include "pimpl/pimplTest.h"

int main() {
    // test arch design

    std::cout << "test arch design" << std::endl;
    design::pimplTest pimplTest;
    pimplTest.process();

    design::facadeTest facadeTest;
    facadeTest.run();

    std::cout << "test end" << std::endl;
}