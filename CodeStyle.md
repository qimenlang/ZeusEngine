代码风格 类Google风格

clang-format配置：
    # BasedOnStyle google
    linux\windows都运行以下脚本生成 .clang-format
    clang-format --style=Google --dump-config > .clang-format
    # IndentWidth Set to 4(唯一改动)


类名 class FileReader  

类方法 processData()  非google风格

成员变量  size_t m_entry_count; 非google风格

函数名  process_data() 

局部变量 float delta_time;

常量 const int kMaxSize = 100;  

枚举类  enum class HttpCode  

枚举值  kNotFound, kInternalError  

文件名  url_parser.cpp