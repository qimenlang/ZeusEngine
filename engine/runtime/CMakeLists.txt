set(TARGET_NAME ZeusRuntime)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp *.c)

#source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME}  ${HEADER_FILES} ${SOURCE_FILES})
#add_library(${TARGET_NAME}   ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME} glfw)

target_include_directories(${TARGET_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)