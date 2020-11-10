cmake_minimum_required(VERSION 3.9)

project(terps)

include_directories(
	${CORE_INCLUDE_DIR}
)

subdirs(
   echo
   tads2
)