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
    PathPlanner.cpp CarPathPlanner.cpp DronePathPlanner.cpp 
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

if(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "15.0")
    set(CMAKE_XCODE_GENERATE_SCHEME ON)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES")
    
    target_link_libraries(CORE PUBLIC
        "-framework CoreFoundation"
        "-framework AppKit"
    )
endif()

target_compile_options(CORE PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang>:-Wall -Wextra -g>
)

set_target_properties(CORE PROPERTIES
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17"
    XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
    XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL "0"
)