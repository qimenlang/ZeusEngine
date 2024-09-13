cmake_minimum_required (VERSION 3.15)

project ("ZeusEngine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ZEUS_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_definitions("ZEUS_ROOT_DIR=${ZEUS_ROOT_DIR}")

# 包含子项目。
add_subdirectory ("thirdparty")
add_subdirectory ("engine")


