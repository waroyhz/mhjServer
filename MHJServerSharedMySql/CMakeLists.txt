cmake_minimum_required(VERSION 3.3)
project(MHJServerSharedMySql)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_DEFINITIONS(-DLINUX)

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_VERBOSE_MAKEFILE ON)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
#MESSAGE(${LIBRARY_OUTPUT_PATH} "CURL library not found")

include_directories(header
        DbDefine
        database
        ../MHJSpserverLib/Header
        /usr/local/include/hiredis
        ../MHJServerShared/header
        ../MHJServerShared/Protocol
        )

set(SOURCE_FILES
    database/migration/MHJDatabase-odb.cxx
    database/migration/MHJDatabase-odb.hxx
    database/MHJDatabase.hxx
    database/MHJSqlTracer.h
    DbDefine/JsonAdminEntity.cpp
    DbDefine/JsonAdminEntity.h
    DbDefine/JsonStrObjMap.cpp
    DbDefine/JsonStrObjMap.h
    DbDefine/MHJDBInterface.cpp
    DbDefine/MHJDBInterface.h
    DbDefine/MHJSubscribe.cpp
    DbDefine/MHJSubscribe.h
    DbDefine/MoAdminEntity.h
    DbDefine/MoDbKey.cpp
    DbDefine/MoDbKey.h
    DbDefine/MoMhjdeviceactivation.h
    DbDefine/MoMhjdeviceauthorize.h
    DbDefine/MoMhjdevicebase.h
    DbDefine/MoMhjdevicechild.h
    DbDefine/MoMHJDeviceVirtualDefine.h
    DbDefine/MoMhjUserEntity.h
    DbDefine/QueueNode.h
    DbDefine/RedisQueueManage.cpp
    DbDefine/RedisQueueManage.h
    DbDefine/ServiceNum2KeyMap.h
    header/md5.h
    header/MHJDataBaseFactory.h
    header/MHJDBInterfaceFactory.h
    header/MHJRedisBaseFactory.h
    header/MHJRedisBlpop.h
    header/MHJRedisCmd.h
    header/MHJRedisSubscribeFactory.h
    header/MHJRedisSubscribeProcessFactory.h
    header/MHJSecurityFactoryClient.h
    header/MHJSecurityFactoryServer.h
    header/MHJSecurityManageAdmin.h
    header/MHJSecurityManageAPP.h
    header/MHJSecurityManageDevice.h
    header/MHJSecurityManageFactory.h
    header/MHJSecurityManageHost.h
    header/MHJSecurityManageServer.h
    header/ProcessTransformationRedis.h
    src/md5.cpp
    src/MHJDataBaseFactory.cpp
    src/MHJDBInterfaceFactory.cpp
    src/MHJRedisBaseFactory.cpp
    src/MHJRedisBlpop.cpp
    src/MHJRedisCmd.cpp
    src/MHJRedisSubscribeFactory.cpp
    src/MHJRedisSubscribeProcessFactory.cpp
    src/MHJSecurityFactory.cpp
    src/MHJSecurityFactoryClient.cpp
    src/MHJSecurityFactoryServer.cpp
    src/MHJSecurityManageAdmin.cpp
    src/MHJSecurityManageAPP.cpp
    src/MHJSecurityManageDevice.cpp
    src/MHJSecurityManageFactory.cpp
    src/MHJSecurityManageHost.cpp
    src/MHJSecurityManageServer.cpp
    src/ProcessTransformationRedis.cpp
    )

#add_executable(MHJServerSharedMySql ${SOURCE_FILES})
add_library(MHJServerSharedMySql ${SOURCE_FILES})

target_link_libraries(MHJServerSharedMySql event)
