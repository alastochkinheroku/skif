cmake_minimum_required(VERSION 3.11)

project(skif)

set(CORE_INCLUDE_DIR 
    ${PROJECT_SOURCE_DIR}/core)

subdirs(
   core
   #3rdparty
   #terps
   tests
)