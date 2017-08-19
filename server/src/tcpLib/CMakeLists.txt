cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Autobon.ai - TCP Library")

set(AB_TCP_LIB_NAME abTcplib)

message(STATUS "= Building ${DESCRIPTION}")

include_directories(include ../include ${CAN_CONFIG_BIN_INCLUDE})

#set( tcp_SRCS src/abTcpSocket.cpp )

# Include *.h files to VS or Qt IDE can edit
file(GLOB tcp_SRCS src/*.cpp include/*.h)

list_files( "${tcp_SRCS}" )

add_library(${AB_TCP_LIB_NAME} ${tcp_SRCS})

target_include_directories(${AB_TCP_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties(${AB_TCP_LIB_NAME}
        PROPERTIES OUTPUT_NAME ${AB_TCP_LIB_NAME})

set_target_properties(${AB_TCP_LIB_NAME}
        PROPERTIES VERSION 1.0 SOVERSION 1)

set( AB_TCP_LIB_CONFIG_NAME ${AB_TCP_LIB_NAME})

install(TARGETS ${AB_TCP_LIB_NAME}
        EXPORT ${AB_TCP_LIB_CONFIG_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(DEFINE_BUILD_SDK)
    install(DIRECTORY include/
            DESTINATION include)

    install(EXPORT ${AB_TCP_LIB_CONFIG_NAME}
            DESTINATION share)
endif(DEFINE_BUILD_SDK)
