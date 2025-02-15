set(TARGET_NAME ZeusRuntime)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.c)

add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# set(ZEUS_ROOT_DIR1 "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(${TARGET_NAME} PRIVATE ZEUS_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/resources\")

target_link_libraries(${TARGET_NAME} glfw stb)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
