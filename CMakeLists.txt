cmake_minimum_required(VERSION 3.20)
project(fractals)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(dep/STX)
add_subdirectory(dep/spdlog)

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

add_executable(fractals ${FR_SRCS} ${FR_HEADERS})

target_include_directories(fractals PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(fractals PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(fractals PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(fractals PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(fractals PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(fractals PUBLIC stx::include)
target_include_directories(fractals PUBLIC spdlog)

target_link_libraries(fractals
        ${SDL2_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES})
target_link_libraries(fractals stx)
target_link_libraries(fractals spdlog)
