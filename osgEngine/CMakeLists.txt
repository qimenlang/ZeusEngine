set(OSG_ENGINE_NAME osgEngine)
add_executable(${OSG_ENGINE_NAME} osgEngine.cpp)
target_link_libraries(${OSG_ENGINE_NAME} PRIVATE osgViewer osgDB osgGA osgUtil osg)
target_compile_definitions(${OSG_ENGINE_NAME} PRIVATE  # 设置静态链接 OSG 和 OpenThreads
    OSG_LIBRARY_STATIC
    OPENTHREADS_STATIC
)