cmake_minimum_required(VERSION 3.20)
project(opengl LANGUAGES CXX C)

add_executable(opengl
        main.cpp
        ImageLoader.cpp
        stb_image.h
        Shader.h
        Shader.cpp
        Camera.h
        Camera.cpp
        Mesh.h
        Mesh.cpp
        Model.h
        Model.cpp
        GUIRender.h
        GUIRender.cpp
        GUI.h
        GUI.cpp
        TextRenderer.h
        TextRenderer.cpp
        InputManager.cpp
        InputManager.h)
target_sources(opengl
        PRIVATE
        main.cpp
)

find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(freetype-gl CONFIG REQUIRED)

target_link_libraries(opengl PRIVATE glfw OpenGL::GL glad::glad glm::glm assimp::assimp freetype-gl)
target_compile_features(opengl PRIVATE cxx_std_20)
