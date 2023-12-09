cmake_minimum_required(VERSION 3.14)

project(OpenGLApp)

find_package(glfw3)

add_subdirectory(glm)

add_executable(app ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/stb_image.cpp app.cpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_link_libraries(app glfw GL glm)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex.glsl shaders/vertex.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment.glsl shaders/fragment.glsl COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/textures/container.jpg textures/container.jpg COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/textures/awesomeface.png textures/awesomeface.png COPYONLY)
