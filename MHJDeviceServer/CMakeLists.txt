cmake_minimum_required(VERSION 3.3)
project(MHJDeviceServer)
add_definitions(-DLINUX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_C_COMPILER       g++ )
MESSAGE("currentPath=" ${PROJECT_SOURCE_DIR})

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(
        header
        header/DeviceProcess
        ../MHJSpserverLib/Header
        ../MHJServerShared/Protocol
        ../MHJServerShared/header
        ../MHJServerShared/header/Tools
        ../MHJServerSharedMySql/header
        ../MHJServerSharedMySql/database
        ../MHJServerSharedMySql/DbDefine
        /usr/local/include/hiredis
        src/DeviceProcess/mqtt
        )

aux_source_directory(src cpp_src)
aux_source_directory(src/DeviceProcess cpp_DeviceProcess)
aux_source_directory(src/DeviceProcess/mqtt cpp_mqtt)



#add_subdirectory(../MHJSpserverLib)
#add_subdirectory(../MHJServerShared)
#add_subdirectory(../MHJServerSharedMySql)
#LINK_DIRECTORIES(../../../bin/lib)

LINK_DIRECTORIES(/usr/local/lib)

add_executable(
        MHJDeviceServer
        ${cpp_src}
        ${cpp_DeviceProcess}
        ${cpp_mqtt}
        )
# 添加链接库
target_link_libraries(MHJDeviceServer pthread event hiredis MHJServerShared MHJServerSharedMySql MHJSpserverLib boost_system json curl boost_filesystem boost_date_time mosquitto)
#MHJSpserverLib MHJServerShared MHJServerSharedMySql

