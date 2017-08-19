cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Autobon.ai - App Library")

message(STATUS "= Building ${DESCRIPTION}")

set(AB_APP_LIB_NAME abApplib)

#set(app_SRCS   src/abIApplication.cpp
#               src/abIApplicationPrivate.cpp )

# Include *.h files to VS or Qt IDE can edit
file(GLOB app_SRCS src/*.cpp include/*.h)

list_files( "${app_SRCS}" )

include_directories(include ../include)

add_library(${AB_APP_LIB_NAME} ${app_SRCS})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/inConfig/abAppTools.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/abAppTools.h @ONLY)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/inConfig/abAppToolsBuildType.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/abAppToolsBuildType.h @ONLY)


target_include_directories(${AB_APP_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

set_target_properties(${AB_APP_LIB_NAME}
        PROPERTIES OUTPUT_NAME ${AB_APP_LIB_NAME})

set_target_properties(${AB_APP_LIB_NAME}
        PROPERTIES VERSION 1.0 SOVERSION 1)

set( AB_APP_LIB_CONFIG_NAME ${AB_APP_LIB_NAME})

if(DEFINE_BUILD_SDK)
    install(TARGETS ${AB_APP_LIB_NAME}
            EXPORT ${AB_APP_LIB_CONFIG_NAME}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

    install(DIRECTORY include/
            DESTINATION include)

    install(EXPORT ${AB_APP_LIB_CONFIG_NAME}
            DESTINATION share)
endif(DEFINE_BUILD_SDK)

