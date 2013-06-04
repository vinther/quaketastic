project(quaketastic C CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost COMPONENTS system thread filesystem log REQUIRED)
find_package(Lua51 REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(Cg REQUIRED)

include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${FTGL_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CG_INCLUDE_PATH})

file(GLOB_RECURSE SRC_LIST src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${FTGL_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${CG_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${CG_GL_LIBRARY})






