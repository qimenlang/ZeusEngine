set(TARGET_NAME multiThread)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(Threads REQUIRED)
#Threads::Threads是跨平台的线程库抽象
target_link_libraries(${TARGET_NAME} PRIVATE utils Threads::Threads)
