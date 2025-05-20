cmake_minimum_required (VERSION 3.15)

set(TARGET_NAME UnitTest)


# file:
#    Generate a list of files that match the <globbing-expressions> and store it into the <variable>. 
#    best practice :GLOB_RECURSE ; The GLOB_RECURSE mode will traverse all the subdirectories of the matched directory and match the files.
file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# find_package(RTTR CONFIG REQUIRED Core)
# 查找 RTTR
# find_package(RTTR::Core REQUIRED)

add_executable (${TARGET_NAME} ${EDITOR_SOURCES}) 

target_link_libraries(${TARGET_NAME} PUBLIC Catch2::Catch2WithMain RTTR::Core)

# 自动复制 DLL 到输出目录
# get_target_property(RTTR_DLL_PATH RTTR::Core IMPORTED_LOCATION_DEBUG)
# add_custom_command(
#     TARGET ${TARGET_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${RTTR_DLL_PATH}"
#     $<TARGET_FILE_DIR:UnitTest>
# )
