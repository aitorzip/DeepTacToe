cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Autobon.ai - UDP Library")

set(AB_UDP_LIB_NAME abUdplib)

message(STATUS "= Building ${DESCRIPTION}")

include_directories(include ../include ${CAN_CONFIG_BIN_INCLUDE})

#set( udp_SRCS src/abUdpSocket.cpp )

# Include *.h files to VS or Qt IDE can edit
file(GLOB udp_SRCS src/*.cpp include/*.h)

list_files( "${udp_SRCS}" )

add_library(${AB_UDP_LIB_NAME} ${udp_SRCS})

target_include_directories(${AB_UDP_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties(${AB_UDP_LIB_NAME}
        PROPERTIES OUTPUT_NAME ${AB_UDP_LIB_NAME})

set_target_properties(${AB_UDP_LIB_NAME}
        PROPERTIES VERSION 1.0 SOVERSION 1)

set( AB_UDP_LIB_CONFIG_NAME ${AB_UDP_LIB_NAME})

install(TARGETS ${AB_UDP_LIB_NAME}
        EXPORT ${AB_UDP_LIB_CONFIG_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(DEFINE_BUILD_SDK)
    install(DIRECTORY include/
            DESTINATION include)

    install(EXPORT ${AB_UDP_LIB_CONFIG_NAME}
            DESTINATION share)
endif(DEFINE_BUILD_SDK)
