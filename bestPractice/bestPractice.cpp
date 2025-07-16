#include <iostream>

#include "cpp/cppQuiz/cppQuiz.hpp"
#include "cpp/cppWeekly/cppWeekly.h"
#include "cpp/features/cppFeature.h"
#include "design/pimpl/pimplTest.h"

int main() {
    // test arch design
    {
        std::cout << "test arch design" << std::endl;
        design::pimplTest pimplTest;
        pimplTest.process();
    }

    // test cpp Features
    {
        cppFeature::run();
    }

    // cpp Weekly
    cppWeekly::run();
    // cpp Quiz
    cppQuiz::run();

    std::cout << "test end" << std::endl;
}