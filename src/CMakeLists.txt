cmake_minimum_required(VERSION 3.9)

project(skif)

if(MSVC)
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			add_compile_options("/std:c++latest")
		endif()
	endif()
endif(MSVC)

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
   frontend
   terps
   tests
)