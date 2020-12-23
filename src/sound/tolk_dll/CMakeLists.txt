cmake_minimum_required(VERSION 3.9)

project(tolk_dll)

set(HEADERS
  Tolk.h
  tolk_dll.h
)
    
set(SOURCES
  tolk_dll.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})