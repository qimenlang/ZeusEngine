cmake_minimum_required (VERSION 3.15)

project ("ZeusEngine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含子项目。
add_subdirectory ("thirdparty")
# 启用测试
enable_testing()
add_subdirectory("utils")
add_subdirectory ("engine")

add_subdirectory("bestPractice")

add_subdirectory("tool")
