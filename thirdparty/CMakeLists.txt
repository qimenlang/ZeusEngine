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


# 构建glslang库并生成package config shim,用于VSG的find_package(glslang CONFIG)
set(GLSLANG_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("glslang")
# Remove stale install-layout package config if a prior configure generated it
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/glslang/glslang-config.cmake"
           "${CMAKE_CURRENT_BINARY_DIR}/glslang/glslang-config-version.cmake")

# VSG uses find_package(glslang CONFIG), not add_subdirectory targets directly.
# glslang as a nested project does not expose a usable build-tree package config,
# so register one that reuses the targets already created above.
set(_ZEUS_GLSLANG_CMAKE_DIR "${CMAKE_CURRENT_BINARY_DIR}/cmake-packages/lib/cmake/glslang")
file(MAKE_DIRECTORY "${_ZEUS_GLSLANG_CMAKE_DIR}")
file(WRITE "${_ZEUS_GLSLANG_CMAKE_DIR}/glslang-config.cmake" [=[
# Build-tree glslang package shim for VulkanSceneGraph
if(NOT TARGET glslang::glslang)
    message(FATAL_ERROR "glslang targets missing; add_subdirectory(glslang) must come first")
endif()
]=])
set(glslang_DIR "${_ZEUS_GLSLANG_CMAKE_DIR}" CACHE PATH "In-tree glslang package for VSG" FORCE)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/cmake-packages")

# vsg install(EXPORT vsgTargets) requires link dependencies in the same export set
install(TARGETS glslang glslang-default-resource-limits SPIRV
    EXPORT vsgTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

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

add_subdirectory("VulkanSceneGraph")
add_subdirectory("vsgXChange")
add_subdirectory("vsgImgui")
add_subdirectory("vsgExamples")
