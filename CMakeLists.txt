cmake_minimum_required(VERSION 3.21)
project(physics)

set(CMAKE_CXX_STANDARD 23)

add_executable(physics main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(physics sfml-system sfml-window sfml-graphics sfml-audio sfml-network)