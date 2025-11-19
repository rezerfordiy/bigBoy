cmake_minimum_required(VERSION 3.10)
project(CORE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)

add_library(CORE STATIC
    AStar.cpp Dubins.cpp Graph.cpp GraphManager.cpp 
    Obstacle.cpp ObstacleManager.cpp RangedDubinsSolver.cpp 
    PathFindingTask.cpp CutFindingTask.cpp Solver.cpp PathData.cpp
)

target_include_directories(CORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
)

target_link_libraries(CORE PUBLIC
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
)

target_link_options(CORE PUBLIC ${YAML_CPP_LDFLAGS})