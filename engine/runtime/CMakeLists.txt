set(ZEUS_RUNTIME_NAME ZeusRuntime)
set(ZEUS_ENGINE_NAME ZeusEngine)


file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.c *.cpp)

message("####"${CMAKE_CURRENT_SOURCE_DIR})

add_library(${ZEUS_RUNTIME_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_compile_definitions(${ZEUS_RUNTIME_NAME} PRIVATE ZEUS_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/resources\")
target_link_libraries(${ZEUS_RUNTIME_NAME} glfw stb glm assimp utils imgui)
target_include_directories(${ZEUS_RUNTIME_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${ZEUS_ENGINE_NAME} main.cpp)
target_link_libraries(${ZEUS_ENGINE_NAME} ${ZEUS_RUNTIME_NAME})
target_include_directories(${ZEUS_ENGINE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 添加 Vulkan 头文件路径
# 假设你的 submodule 在 external/Vulkan-Headers
# include_directories(external/Vulkan-Headers/include)
# # 查找系统上安装的 Vulkan 库${CMAKE_CURRENT_SOURCE_DIR}
find_package(Vulkan REQUIRED)
add_executable(VulkanStart vulkanStart.cpp)
target_compile_definitions(VulkanStart PRIVATE ZEUS_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/resources\")
target_link_libraries(VulkanStart glfw stb glm assimp utils Vulkan::Vulkan)
target_include_directories(VulkanStart PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/Vulkan-Headers/include)
