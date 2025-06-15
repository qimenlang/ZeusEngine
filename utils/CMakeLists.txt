set(TARGET_NAME utils)

# 创建库目标
add_library(utils INTERFACE)
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

