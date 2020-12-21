cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(skif-gui)
set(wxWidgets_USE_LIBS)
find_package(wxWidgets REQUIRED)

include(${wxWidgets_USE_FILE})

include_directories(${wxWidgets_INCLUDE_DIRS})
link_directories(${wxWidgets_LIBRARY_DIRS})

set(SOURCES main.cpp
 mainframe.cpp
 centralpanel.cpp
 parserlineinput.cpp
 logframe.cpp
)

set(HEADERS mainframe.h
centralpanel.h
parserlineinput.h
logframe.h
)

add_executable (${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDE_DIR})
target_link_libraries (${PROJECT_NAME} ${wxWidgets_LIBRARIES} core echo tads2terp tads2 sound_bass 
${BASS_WIN_LINK_DIR}/bass.lib 
${BASS_WIN_LINK_DIR}/bassmidi.lib
${BASS_WIN_LINK_DIR}/bassmix.lib
${BASS_WIN_LINK_DIR}/bassenc.lib
${BASS_WIN_LINK_DIR}/bassenc_ogg.lib)

#копируем DLL в выходной каталог
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${BASS_WIN_DLL_DIR}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)