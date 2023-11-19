cmake_minimum_required(VERSION 3.14)

project(OpenGLApp)

find_package(glfw3)

add_executable(app ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c app.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_link_libraries(app glfw GL)
