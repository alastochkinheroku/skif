cmake_minimum_required(VERSION 3.11)

project(echo)

set(HEADERS
  EchoEngine.h
)
    
set(SOURCES
  EchoEngine.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})