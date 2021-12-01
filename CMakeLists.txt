cmake_minimum_required(VERSION 3.20)
project(fractals)
set(CMAKE_POLICY_DEFAULT_CMP0054 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(dep/STX)
add_subdirectory(dep/spdlog)
add_subdirectory(dep/SDL)

set(FR_HEADERS
        src/window.h
        src/gfx/vbo.h
        src/gfx/vertexarray.h
        src/gfx/shader.h
        src/gfx/shaderprogram.h
        src/common.h
        src/maths/mat4.h
        src/gfx/texture2d.h
        src/gfx/ebo.h)

set(FR_SRCS
        src/gfx/vbo.cpp
        src/gfx/vertexarray.cpp
        src/gfx/shader.cpp
        src/gfx/shaderprogram.cpp
        src/main.cpp
        src/window.cpp
        src/gfx/ebo.cpp)

set(IMGUI_HDRS
        src/util/imconfig.h
        src/util/imgui.h
        src/util/imgui_internal.h
        src/util/imstb_rectpack.h
        src/util/imstb_textedit.h
        src/util/imstb_truetype.h
        src/util/backends/imgui_impl_sdl.h
        src/util/backends/imgui_impl_opengl3.h
        src/util/backends/imgui_impl_sdlrenderer.h)

set(IMGUI_SRCS
        src/util/imgui.cpp
        src/util/imgui_draw.cpp
        src/util/imgui_tables.cpp
        src/util/imgui_widgets.cpp
        src/util/backends/imgui_impl_opengl3.cpp
        src/util/backends/imgui_impl_sdl.cpp
        src/util/backends/imgui_impl_sdlrenderer.cpp)

add_executable(fractals ${FR_SRCS} ${FR_HEADERS} ${IMGUI_SRCS} ${IMGUI_HDRS})

target_include_directories(fractals PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(fractals PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(fractals PUBLIC OpenGL::GL)
target_include_directories(fractals PUBLIC GLEW::glew)
target_include_directories(fractals PUBLIC stx::include)
target_include_directories(fractals PUBLIC spdlog)
target_include_directories(fractals PUBLIC SDL2::Video)

target_link_libraries(fractals
        GLEW::glew
        OpenGL::GL
        ${CMAKE_DL_LIBS})

target_link_libraries(fractals stx)
target_link_libraries(fractals spdlog)
target_link_libraries(fractals SDL2)
