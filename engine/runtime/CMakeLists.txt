set(ZEUS_RUNTIME_NAME ZeusRuntime)
set(ZEUS_ENGINE_NAME ZeusEngine)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.c *.cpp)

add_library(${ZEUS_RUNTIME_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(${ZEUS_RUNTIME_NAME} PRIVATE ZEUS_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/resources\")

target_link_libraries(${ZEUS_RUNTIME_NAME} glfw stb glm assimp utils)

target_include_directories(${ZEUS_RUNTIME_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_include_directories(
#   ${module_name}
#   PUBLIC include
#   PRIVATE src)



add_executable(${ZEUS_ENGINE_NAME} main.cpp)

target_link_libraries(${ZEUS_ENGINE_NAME} ${ZEUS_RUNTIME_NAME})

target_include_directories(${ZEUS_ENGINE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

