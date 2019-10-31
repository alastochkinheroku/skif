cmake_minimum_required(VERSION 3.11)

project(skif)

if(UNIX)
   set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
   set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
   set(CMAKE_CXX_STANDARD 11)
   set(PTHREAD_LIBRARY "-Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
endif()

set(CORE_INCLUDE_DIR 
    ${PROJECT_SOURCE_DIR}/core)

subdirs(
   core
   #3rdparty
   #terps
   tests
)