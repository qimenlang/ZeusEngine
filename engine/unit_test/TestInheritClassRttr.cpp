/************************************************************************************
 *                                                                                   *
 *   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org> *
 *                                                                                   *
 *   This file is part of RTTR (Run Time Type Reflection) * License: MIT License
 **
 *                                                                                   *
 *   Permission is hereby granted, free of charge, to any person obtaining * a
 *copy of this software and associated documentation files (the "Software"), *
 *   to deal in the Software without restriction, including without limitation *
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense, *
 *   and/or sell copies of the Software, and to permit persons to whom the *
 *   Software is furnished to do so, subject to the following conditions: *
 *                                                                                   *
 *   The above copyright notice and this permission notice shall be included in
 ** all copies or substantial portions of the Software. *
 *                                                                                   *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *FROM,   * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *DEALINGS IN THE   * SOFTWARE. *
 *                                                                                   *
 *************************************************************************************/
#include <catch2/catch_test_macros.hpp>
#include <rttr/registration>
#include <rttr/registration_friend>
#include <iostream>
#include "test_classes.h"
using namespace rttr;

class Base
{
    RTTR_ENABLE()
private:
    /* data */
public:
    Base(/* args */){};
    ~Base(){};
};

class Sub1 : public Base
{
    RTTR_ENABLE(Base)
private:
    /* data */
public:
    Sub1(/* args */){};
    ~Sub1(){};
};

class Sub2 : public Sub1
{
    RTTR_ENABLE(Sub2)
private:
    /* data */
public:
    Sub2(/* args */){};
    ~Sub2(){};
};



TEST_CASE("rttr get clase type", "[rttrGetType]") {
    Sub1 s;
    REQUIRE(s.get_type()==type::get<Sub1>());
    std::cout<<"rttrGetType :s.get_type() == "<<s.get_type().get_name().to_string().c_str()<<std::endl;
    

    Base b;
    REQUIRE(b.get_type()==type::get<Base>());
    std::cout<<"rttrGetType :b.get_type() == "<<b.get_type().get_name().to_string().c_str()<<std::endl;

}


TEST_CASE("Test rttr::type - SingleClassInheritance", "[cast]")
{

    {
        ClassSingle4A instance6A;
        ClassSingleBase& baseSingle = instance6A;

        // down cast cast
        CHECK(rttr_cast<ClassSingle6A*>(&baseSingle) == nullptr);
        CHECK(rttr_cast<ClassSingle3A*>(&baseSingle) != nullptr);
        CHECK(rttr_cast<ClassSingle6B*>(&baseSingle) == nullptr);

        // up cast cast
        CHECK(rttr_cast<ClassSingleBase*>(&instance6A) != nullptr);
        CHECK(rttr_cast<ClassSingle3A*>(&instance6A) != nullptr);
        CHECK(rttr_cast<ClassSingle1A*>(&instance6A) != nullptr);

         // 获取对象实际类型
        rttr::type t = rttr::type::get(baseSingle);
        REQUIRE(t == type::get<ClassSingle4A>());
        REQUIRE(baseSingle.get_type()==type::get<ClassSingle4A>());
        std::cout << "baseSingle : " << t.get_name().to_string() << std::endl;

        ClassSingle4A* obj4 = new ClassSingle4A();
        REQUIRE(rttr::rttr_cast<ClassSingle4A*>(obj4)!=nullptr);
        delete obj4;
        REQUIRE(obj4!=nullptr);
    }

    {
        ClassSingle6E instance6E;
        ClassSingleBase& baseSingle = instance6E;

        // down cast cast
        CHECK(rttr_cast<ClassSingle6A*>(&baseSingle) == nullptr);
        CHECK(rttr_cast<ClassSingle3E*>(&baseSingle) != nullptr);
        CHECK(rttr_cast<ClassSingle6E*>(&baseSingle) != nullptr);

        // up cast cast
        CHECK(rttr_cast<ClassSingleBase*>(&instance6E) != nullptr);
        CHECK(rttr_cast<ClassSingle3E*>(&instance6E) != nullptr);
        CHECK(rttr_cast<ClassSingle1E*>(&instance6E) != nullptr);
        CHECK(rttr_cast<ClassSingle6E*>(&instance6E) != nullptr);
    }
}
