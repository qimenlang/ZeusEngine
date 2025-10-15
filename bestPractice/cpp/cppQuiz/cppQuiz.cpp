#include "cppQuiz.hpp"

#include <type_traits>

// 显示特化模板
// 放在头文件中时，会被多个包含该头文件的源文件编译，导致重复定义
template <>
void quiz1_1(const int &i) {
    PRINT_FUNC_NAME();
}

void cppQuiz1() {
    PRINT_FUNC_NAME();  // i 是 int 类型的非常量左值
    int i = 42;

    // 主模板是精确匹配、特化模板需要类型转换（int&->const int&
    // C++重载规则，优先选择不需要类型转换的精确匹配
    quiz1_1(i);
    // 核心结论
    // 1.模板特化不参与重载决议：编译器先选择最匹配的模板，再检查是否有特化
    // 2.常量性影响匹配：const 修饰符会导致不同的函数签名
    // 3.精确匹配优先：C++ 总是优先选择需要最少隐式转换的版本
    quiz1_2(i);
    std::cout << std::endl;
}

void quiz2(const std::string &) { PRINT_FUNC_NAME(); }
void quiz2(const char *) { PRINT_FUNC_NAME(); }
void quiz2(const void *) { PRINT_FUNC_NAME(); }

void cppQuiz2() {
    PRINT_FUNC_NAME();
    // 字符串标量并不是 std::string ,而是 const char[],若要匹配quiz_2(const
    // string&),必须经历一次到string的隐式转换
    quiz2("foo");
    const char *bar = "bar";
    quiz2(bar);
    const void *bar2 = "bar2";
    quiz2(bar2);
    quiz2(std::string("string"));
    std::cout << std::endl;
}

void cppQuiz29() {
    PRINT_FUNC_NAME();
    quiz29A a;
    a.bar();  // 输出1
    // 构造b时，先调用父类构造函数quiz29A()，
    // 此时B的部分未初始化，调用派生类函数可能访问未初始化的成员,因此调用A的foo()，输出1
    quiz29B b;
    b.bar();  // 输出2
    // 析构b时，B无析构函数，调用父类析构函数quiz29A()，此时B的部分已析构，调用A的foo()，输出1
    std::cout << std::endl;
}

void cppQuiz31() {
    PRINT_FUNC_NAME();
    // 烦人的解析:most vexing parse
    //  以下创建y对象的方式，实际是声明了一个返回Quiz31Y类型，参数为函数指针的的函数y；
    //  参数是一个函数指针，该函数指针无参且返回Quiz31X类型；
    //  Quiz31Y y(Quiz31X());
    // 创建 Quiz31X 对象，需要使用初始化列表，避免直接调用无参构造函数;
    Quiz31Y y(Quiz31X{});
    y.f();
    std::cout << std::endl;
}

void cppQuiz38() {
    PRINT_FUNC_NAME();
    /*
    如果表达式是变量名（如 decltype(a)），则推导出变量的声明类型（这里是 int）。
    如果表达式带有额外括号（如 decltype((a))），则它被视为一个左值表达式（lvalue
    expression）。对于左值表达式，decltype 推导出类型的引用（reference）。
    */
    int a = 0;
    decltype(a) b = a;  // b 是 int 类型
    decltype((a)) c =
        a;  // c 是 int& 类型，因为decltype((a)) 返回的是 a 的引用类型
    c++;
    // 检查 c 的类型是否是 int&
    if (std::is_same_v<decltype(c), int &>)
        std::cout << "c is int&" << std::endl;
    std::cout << a << b << c;  // 输出 101
    std::cout << std::endl;
}

void cppQuiz42() {
    PRINT_FUNC_NAME();
    cppQuiz42A a1;
    cppQuiz42A
        a2{};  // 使用{}初始化，优先匹配默认构造函数,而非std::initializer_list构造函数
    cppQuiz42A a3{
        1};  // 非空初始化列表，优先匹配std::initializer_list构造函数，即便其他构造函数更匹配
    cppQuiz42A a4{1, 2};
    std::cout << std::endl;
}

// quiz109

void cppQuiz109() {
    PRINT_FUNC_NAME();
    auto print = [](int x) { std::cout << x; };
    // C++规则：模板类型推导严格基于实参的静态类型，不考虑构造函数或转换操作符的隐式转换
    // 编译失败：lambda的闭包类型无法直接匹配std::function<void(T)>模板参数，类型推导失败
    //  call_with109(print, 42);
    //  显示指定模板类型T = int,
    call_with109<int>(print, 42);
    // lambda与std::function不能隐式转换；强制转换 <lambda(int)>为
    // std::function<void(int)>;
    call_with109(static_cast<std::function<void(int)>>(print), 42);
    // F模板类型推导为lambda的闭包类型，T模板类型推导为int
    call_with109_2(print, 42);
    std::cout << std::endl;
}

// quiz116
// 引用
int y116(int &) { return 1; }
// 右值引用
int y116(int &&) { return 2; }

int j116(int &&t) { return y116(t); }

void cppQuiz116() {
    PRINT_FUNC_NAME();
    int x = 0;
    std::cout << f116(x) << f116(1) << std::endl;  // 输出12
    std::cout << g116(x) << g116(1) << std::endl;  // 输出22
    std::cout << h116(x) << h116(1) << std::endl;  // 输出12
    std::cout << j116(1) << std::endl;             // 输出12
}

void cppQuiz338() {
    PRINT_FUNC_NAME();
    std::cout << f338<int>() << std::endl;          // 输出0
    std::cout << f338<std::string>() << std::endl;  // 输出空字符串

    std::cout << "Factorial count" << std::endl;
    constexpr uint64_t value = factorial(10);  //编译时计算
    std::cout << "Factorial of 10 is: " << value << std::endl;  // 输出3628800
}

int main() {
    cppQuiz1();
    cppQuiz2();
    cppQuiz29();
    cppQuiz31();
    cppQuiz38();
    cppQuiz42();
    cppQuiz109();
    cppQuiz116();
    cppQuiz338();
    return 0;
}
