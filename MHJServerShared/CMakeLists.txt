cmake_minimum_required(VERSION 3.3)

project(MHJServerShared)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_VERBOSE_MAKEFILE on)

#预定义宏，-D选项
ADD_DEFINITIONS(-DLINUX)

INCLUDE_DIRECTORIES(header
        Protocol
        ../MHJSpserverLib/Header
        )

aux_source_directory(src cpp_src)
aux_source_directory(src/Tools cpp_Tools)
aux_source_directory(Protocol cpp_Protocol)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
#set(Build)
MESSAGE("LIBRARY_OUTPUT_PATH=" ${LIBRARY_OUTPUT_PATH})



#set(SOURCE_FILES
#    header/Tools/HostAddress.h
#    header/Tools/stringTools.cpp
#    header/Tools/stringTools.h
#    header/CClientASIO.h
#    header/HostClientFactory.h
#    header/MHJFuncTimeRecordMap.h
#    header/MHJFunctionTimeRecord.h
#    header/MHJHostClient.h
#    header/MHJInitialize.h
#    header/MHJMsgDecoder.h
#    header/MHJProtocol.h
#    header/MHJSecurityFactory.h
#    header/MHJTimeRecordList.h
#    header/ProcessFactory.h
#    header/ProcessTransformationFactory.h
#    header/Protocol.h
#    header/QueueData.h
#    header/ServerHandler.h
#    header/ServerProcessHandlerFactory.h
#    header/SP_OnlineManager.h
#    Protocol/MHJDefine.h
#    Protocol/MHJSwitchESP8266Define.h
#    Protocol/ProtocolAdmin.h
#    Protocol/ProtocolApp.h
#    Protocol/ProtocolBroadcastDeviceSwitch.h
#    Protocol/ProtocolDeviceBuletoothDoor.h
#    Protocol/ProtocolDeviceCurtain.h
#    Protocol/ProtocolDeviceSwitch.h
#    Protocol/ProtocolHost.h
#    Protocol/ProtocolServer.h
#    Protocol/ProtocolServerRedisDefine.h
#    Protocol/ProtocolTypeDefine.c
#    Protocol/ProtocolTypeDefine.h
#    src/Tools/HostAddress.cpp
#    src/CClientASIO.cpp
#    src/HostClientFactory.cpp
#    src/MHJFunctionTimeRecord.cpp
#    src/MHJHostClient.cpp
#    src/MHJInitilaize.cpp
#    src/MHJMsgDecoder.cpp
#    src/MHJProtocol.cpp
#    src/MHJTimeRecordList.cpp
#    src/ProcessFactory.cpp
#    src/ProcessTransformationFactory.cpp
#    src/Protocol.c
#    src/ServerHandler.cpp
#    src/ServerProcessHandlerFactory.cpp
#    src/SP_OnlineManager.cpp
#    MhJServerShared.cpp
#    MhJServerShared.h)

#add_executable(MHJServerShared ${SOURCE_FILES})
add_library(MHJServerShared ${cpp_src} ${cpp_Tools} ${cpp_Protocol} )