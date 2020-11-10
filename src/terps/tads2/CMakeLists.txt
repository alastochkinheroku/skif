cmake_minimum_required(VERSION 3.9)

project(tads2terp)

add_compile_definitions(T_WIN32)

set(HEADERS
  Tads2Engine.h
  
  os/oswin.h
)
    
set(SOURCES
  Tads2Engine.cpp

  os/inout.c
  os/oem_skif.c
  os/osskifnui.c
  os/osdosex.c
)

subdirs(
   tads2_base
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
  tads2_base
)
target_link_libraries(${PROJECT_NAME} tads2)