set(BUILD_UNIT_TESTS OFF CACHE BOOL "Disable unit tests of rttr" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples of rttr" FORCE)
set(BUILD_DOCUMENTATION OFF CACHE BOOL "Disable docs of rttr" FORCE)
set(BUILD_RTTR_DYNAMIC OFF CACHE BOOL "Disable Build Dynamic Lib of rttr" FORCE)
set(BUILD_STATIC ON CACHE BOOL "Only Build Static Lib of rttr" FORCE)

add_subdirectory("glfw")
add_subdirectory("stb")
add_subdirectory("glm")
add_subdirectory("Catch2")
add_subdirectory("assimp")
set(ENABLE_OPT OFF)
add_subdirectory("glslang")

add_subdirectory("rttr")
# post fix : disable -Werror for rttr targets when using clang
set(RTTR_TARGETS
    rttr_core_lib
    rttr_lib
)
foreach(t ${RTTR_TARGETS})
    if(TARGET ${t})
        target_compile_options(${t} PRIVATE
            $<$<CXX_COMPILER_ID:Clang>:-Wno-error>
            $<$<CXX_COMPILER_ID:Clang>:-Wno-microsoft-unqualified-friend>
        )
    endif()
endforeach()

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui*.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
