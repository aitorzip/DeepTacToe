cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

set(DESCRIPTION "Autobon.ai - Game Protobuf Library")

message(STATUS "= Building ${DESCRIPTION}")

set(AB_PROTO_LIB_NAME abProtolib)

# Include *.h files to VS or Qt IDE can edit
file(GLOB proto_SRC src/*.cpp include/*.h)

list_files( "${proto_SRC}" )

include_directories(include ../include ${PROTOBUF_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(AB_PROTO_SRC PROTO_HEADER ../../../protobuf/abGameServerPB.proto)

add_library(${AB_PROTO_LIB_NAME} ${PROTO_HEADER} ${AB_PROTO_SRC} ${proto_SRC})

target_link_libraries(${AB_PROTO_LIB_NAME} ${Protobuf_LIBRARY} abTcplib)

target_include_directories(${AB_PROTO_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties(${AB_PROTO_LIB_NAME}
        PROPERTIES OUTPUT_NAME ${AB_PROTO_LIB_NAME})

set_target_properties(${AB_PROTO_LIB_NAME}
        PROPERTIES VERSION 1.0 SOVERSION 1)

set( AB_PROTO_LIB_CONFIG_NAME ${AB_PROTO_LIB_NAME})

install(TARGETS ${AB_PROTO_LIB_NAME}
        EXPORT ${AB_PROTO_LIB_CONFIG_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(DEFINE_BUILD_SDK)
    install(DIRECTORY include/
            DESTINATION include)

    install(EXPORT ${AB_PROTO_LIB_CONFIG_NAME}
            DESTINATION share)
endif(DEFINE_BUILD_SDK)
