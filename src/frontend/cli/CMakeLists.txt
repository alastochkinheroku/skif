cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(cli)

set(HEADERS
)

set(SOURCES
    main.cpp
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} core echo tads2terp tads2 sound_bass 
${BASS_WIN_LINK_DIR}/bass.lib 
${BASS_WIN_LINK_DIR}/bassmidi.lib
${BASS_WIN_LINK_DIR}/bassmix.lib
${BASS_WIN_LINK_DIR}/bassenc.lib
${BASS_WIN_LINK_DIR}/bassenc_ogg.lib
)