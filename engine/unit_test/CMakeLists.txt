cmake_minimum_required (VERSION 3.15)

set(TARGET_NAME UnitTest)

# file:
#    Generate a list of files that match the <globbing-expressions> and store it into the <variable>. 
#    best practice :GLOB_RECURSE ; The GLOB_RECURSE mode will traverse all the subdirectories of the matched directory and match the files.
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable (${TARGET_NAME} ${TEST_SOURCES}) 
target_link_libraries(${TARGET_NAME} PUBLIC Catch2::Catch2WithMain RTTR::Core ZeusRuntime)

#CTest
add_executable (testExample ${TEST_SOURCES}) 
target_link_libraries(testExample PUBLIC Catch2::Catch2WithMain RTTR::Core ZeusRuntime)
# 注册测试
add_test(NAME testExample COMMAND testExample -s --verbosity high )

