cmake_minimum_required(VERSION 3.11)

project(core)

set(HEADERS
  EngineManager.h
  GameDescription.h
  INetworkDownloadManager.h
  ITextGameEngine.h
  IUserCommand.h
  SettingsElement.h
  DomainException.h
  GLogger.h
)
    
set(SOURCES
  EngineManager.cpp
  GLogger.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})