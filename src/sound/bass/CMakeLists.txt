cmake_minimum_required(VERSION 3.9)

project(sound_bass)

set(HEADERS
  BassSoundPlayer.h
)
    
set(SOURCES
  BassSoundPlayer.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})