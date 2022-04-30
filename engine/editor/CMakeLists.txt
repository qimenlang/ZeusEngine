cmake_minimum_required (VERSION 3.15)

set(TARGET_NAME ZeusEditor)

# file:
#    Generate a list of files that match the <globbing-expressions> and store it into the <variable>. 
#    best practice :GLOB_RECURSE ; The GLOB_RECURSE mode will traverse all the subdirectories of the matched directory and match the files.
file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
# source_group:
#    Defines a group into which sources will be placed in project files. 
#source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_HEADERS} ${EDITOR_SOURCES} )

add_executable (${TARGET_NAME} ${EDITOR_SOURCES}) 

# target_include_directories:
#   Specifies include directories to use when compiling a given target. 
#   PRIVATE  ???
target_include_directories(${TARGET_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${TARGET_NAME} ZeusRuntime)