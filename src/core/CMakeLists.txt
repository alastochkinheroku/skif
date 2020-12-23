cmake_minimum_required(VERSION 3.11)

project(core)

set(HEADERS
  EngineManager.h
  GameDescription.h
  INetworkDownloadManager.h
  TextGameEngine.h
  IUserOutputNotifier.h
  DomainException.h
  GLogger.h
  ILocalFileStorage.h
  RepositoryProvider.h
  IConfigParser.h
  IOSLayer.h
  ITolk.h
)
    
set(SOURCES
  EngineManager.cpp
  GLogger.cpp
  TextGameEngine.cpp
  RepositoryProvider.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})