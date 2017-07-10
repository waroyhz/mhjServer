cmake_minimum_required(VERSION 3.3)
project(MHJSpserverLib)
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-Dlinux)

include_directories("Header")
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

message(STATUS  "output_path=" ${LIBRARY_OUTPUT_PATH})

set(SOURCE_FILES
    Header/event_msgqueue.h
    Header/spbuffer.hpp
    Header/spdispatcher.hpp
    Header/speventcb.hpp
    Header/spexecutor.hpp
    Header/spgetopt.h
    Header/sphandler.hpp
    Header/sphttp.hpp
    Header/sphttpmsg.hpp
    Header/spiochannel.hpp
    Header/spioutils.hpp
    Header/splfserver.hpp
    Header/spmsgblock.hpp
    Header/spmsgdecoder.hpp
    Header/spporting.hpp
    Header/sprequest.hpp
    Header/spresponse.hpp
    Header/spserver.hpp
    Header/spsession.hpp
    Header/spsmtp.hpp
    Header/spthread.hpp
    Header/spthreadpool.hpp
    Header/sputils.hpp
    Source/event_msgqueue.c
    Source/spbuffer.cpp
    Source/spdispatcher.cpp
    Source/speventcb.cpp
    Source/spexecutor.cpp
    Source/spgetopt.c
    Source/sphandler.cpp
    Source/sphttp.cpp
    Source/sphttpmsg.cpp
    Source/spiochannel.cpp
    Source/spioutils.cpp
    Source/splfserver.cpp
    Source/spmsgblock.cpp
    Source/spmsgdecoder.cpp
    Source/sprequest.cpp
    Source/spresponse.cpp
    Source/spserver.cpp
    Source/spsession.cpp
    Source/spsmtp.cpp
    Source/spthread.cpp
    Source/spthreadpool.cpp
    Source/sputils.cpp)

#add_executable(MHJSpserverLib ${SOURCE_FILES})
add_library(MHJSpserverLib ${SOURCE_FILES})