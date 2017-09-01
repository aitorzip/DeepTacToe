cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Autobon.ai - MariaDB Library")

message(STATUS "= Building ${DESCRIPTION}")

set(AB_MARIA_DB_LIB_NAME abMariaDBlib)

# Include *.h files to VS or Qt IDE can edit
file(GLOB mariadb_SRCS src/*.cpp include/*.h)

list_files( "${mariadb_SRCS}" )

include_directories(include ../include)

add_library(${AB_MARIA_DB_LIB_NAME} ${mariadb_SRCS})

target_include_directories(${AB_MARIA_DB_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties(${AB_MARIA_DB_LIB_NAME}
        PROPERTIES OUTPUT_NAME ${AB_MARIA_DB_LIB_NAME})

set_target_properties(${AB_MARIA_DB_LIB_NAME}
        PROPERTIES VERSION 1.0 SOVERSION 1)

set( AB_MARIA_DB_LIB_CONFIG_NAME ${AB_MARIA_DB_LIB_NAME})

if(DEFINE_BUILD_SDK)
    install(TARGETS ${AB_MARIA_DB_LIB_NAME}
            EXPORT ${AB_MARIA_DB_LIB_CONFIG_NAME}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

    install(DIRECTORY include/
            DESTINATION include)

    install(EXPORT ${AB_MARIA_DB_LIB_CONFIG_NAME}
            DESTINATION share)
endif(DEFINE_BUILD_SDK)

