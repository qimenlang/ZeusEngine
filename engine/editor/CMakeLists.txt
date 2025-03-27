cmake_minimum_required(VERSION 3.15)

set(TARGET_NAME ZeusEditor)

file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${TARGET_NAME} ${EDITOR_SOURCES})

target_include_directories(${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
