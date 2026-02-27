cmake_minimum_required(VERSION 4.1.2)
project(opengl LANGUAGES CXX C)

add_executable(opengl
        ImageLoader.cpp
        Shader.h
        stb_image.h
        Shader.cpp
        Camera.h
        Camera.cpp)
target_sources(opengl
        PRIVATE
        main.cpp
)

find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(opengl PRIVATE glfw OpenGL::GL glad::glad)
target_compile_features(opengl PRIVATE cxx_std_20)
