cmake_minimum_required(VERSION 3.20)
project(cv_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(cv_test main.cpp glad.c test.cpp test.h shaders/Shader.cpp shaders/Shader.h)

include_directories(./include)

link_directories(./lib)

target_link_libraries(cv_test -lopengl32 ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)
