# CMakeLists.txt in live

PROJECT( live555 )

# includes
INCLUDE_DIRECTORIES(
BasicUsageEnvironment/include
groupsock/include
liveMedia/include
UsageEnvironment/include
)

ADD_SUBDIRECTORY(BasicUsageEnvironment)
ADD_SUBDIRECTORY(groupsock)
ADD_SUBDIRECTORY(liveMedia)
ADD_SUBDIRECTORY(UsageEnvironment)
