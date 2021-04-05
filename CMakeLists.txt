cmake_minimum_required(VERSION 3.14)
project(websocket-client)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
else ()
    add_definitions(-DDEBUG=0)
endif()

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
    add_definitions(-D_WIN32_WINNT=0x600)
endif()

set(LIB_PREFIX "/usr/local/lib")

find_package(Threads REQUIRED)

find_package(Libwebsockets)
set(WEBSOCKET_LIB "${LIB_PREFIX}/libwebsockets.a")

find_package(OpenSSL REQUIRED)

set (LIBS
        src/libs/callback.h
        src/libs/http_client.h
        src/libs/http_client.cpp
        src/libs/ring_buffer.h
        src/libs/socket_client.h
        src/libs/socket_client.cpp
        src/libs/socket_service.h
        src/libs/socket_service.cpp
        src/libs/utils.h
        src/libs/websocket_client.h
        src/libs/websocket_client.cpp)

add_executable(websocket-client main.cpp ${LIBS})

target_link_libraries(websocket-client
        ${WEBSOCKET_LIB}
        ${OPENSSL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
)