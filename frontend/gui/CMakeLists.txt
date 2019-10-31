cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(gui)

set(HEADERS
)

set(SOURCES
    main.cpp
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} core)