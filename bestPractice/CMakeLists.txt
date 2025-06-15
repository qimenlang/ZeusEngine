set(TARGET_NAME bestPractice)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES}) 

target_link_libraries(${TARGET_NAME} PRIVATE utils)
