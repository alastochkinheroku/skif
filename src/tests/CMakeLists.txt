cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(UnitTest)

if(UNIX)
   set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
   set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
   set(CMAKE_CXX_STANDARD 11)
endif()


#sources with fakeit library
set(SOURCES_WITH_FAKE
)

#TODO set compiler version, instead of unix
if(UNIX)
   if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
      set(SOURCES_WITH_FAKE "")
   ENDIF()
ENDIF()

set(HEADERS
)                   

set(SOURCES
    CppUnitLite/Failure.cpp
    CppUnitLite/SimpleString.cpp
    CppUnitLite/Test.cpp
    CppUnitLite/TestRegistry.cpp
    CppUnitLite/TestResult.cpp
    
    testEngineManager.cpp
	
    testMain.cpp
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CORE_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} core)

#Запуск юнит-тестов как части проекта
set(UNIT_TEST ${PROJECT_NAME})
add_custom_command(
     TARGET ${UNIT_TEST}
     COMMENT "Run tests"
     POST_BUILD 
     COMMAND ${UNIT_TEST}
)