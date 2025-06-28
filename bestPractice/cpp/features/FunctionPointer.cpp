#include "cppFeature.h"
#include <iostream>
/* 声明函数指针，需要指定指针所指向函数的返回值和参数类型； */
namespace FunctionPointerTest {
// 返回值和参数都是void
void (*functionPointer)(void);

// 返回值为int，参数为int和int
int (*math_op)(int, int);

// 定义函数指针类型
typedef int (*MathOp)(int, int);

// 1. 普通函数声明
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

void test(){
    math_op = add; 
    std::cout<<"3+4 = "<<math_op(3, 4)<<std::endl; // 调用add函数
    math_op = subtract;
    std::cout<<"7-4 = "<<math_op(7, 4)<<std::endl; // 调用add函数
    MathOp mathOp = add; 
    std::cout<<"3+4 = "<<mathOp(3, 4)<<std::endl; // 调用add函数
}
}