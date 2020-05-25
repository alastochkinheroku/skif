cmake_minimum_required(VERSION 3.9)

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
  IGameManager.h
)
    
set(SOURCES
  EngineManager.cpp
  GLogger.cpp
  IGameManager.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})