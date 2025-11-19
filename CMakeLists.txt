cmake_minimum_required(VERSION 3.16)
project(PathPlanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR "/opt/homebrew/opt/qt/lib/cmake/Qt6")
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt;${CMAKE_PREFIX_PATH}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

if(NOT TARGET CORE)
    message(STATUS "Library my_lib not found, building it...")
    add_subdirectory(CORE/CORE)
endif()

set(SOURCES
    main.cpp
    GUI/GUI/Mainwindow.cpp
)

set(HEADERS
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/GUI/GUI
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    CORE
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME}
        "-framework CoreFoundation"
        "-framework AppKit"
        "-framework OpenGL"
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/configs"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/configs"
    COMMENT "Copying configs directory to binary directory"
)