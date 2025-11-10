cmake_minimum_required(VERSION 3.10)
project(my_lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)

add_library(my_lib STATIC
    AStar.cpp Dubins.cpp Graph.cpp GraphManager.cpp 
    Obstacle.cpp ObstacleManager.cpp RangedDubinsSolver.cpp 
    Task1.cpp Task2.cpp Solver.cpp Data.cpp
)

target_include_directories(my_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
)

target_link_libraries(my_lib PUBLIC
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
)

target_link_options(my_lib PUBLIC ${YAML_CPP_LDFLAGS})