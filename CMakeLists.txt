project(quaketastic C CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(Boost COMPONENTS system thread filesystem program_options REQUIRED)
find_package(Lua51 REQUIRED)

aux_source_directory(. SRC_LIST)

file(GLOB SRC_LIST *.cpp *.hpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})






