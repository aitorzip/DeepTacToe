# Sample Cmake file for quick C++ project creation
# By Robert J. Gebis (oxoocoffee)

cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( STATUS "In-source builds not allowed. Please make build directory")
    message( STATUS "and run CMake from there")
    message( STATUS "You may need to remove CMakeCache.txt and CmakeFiles/" )
    message( FATAL_ERROR )
endif()

#-----------------------------------------------------------------------
set(PROJ_NAME     "abGameServer")
set(COMPANY_NAME  "Autobon.ai")

# The version number is used.
set(SVR_VERSION_MAJOR      0)
set(SVR_VERSION_MINOR      1)
set(SVR_VERSION_PATCH      1)
# This one will be set with git revision
set(SVR_VERSION_REVISION   0)

# We set temporary version here since project() must be called
# first to initailize cmake subsystem but we will update version later when we test for git repo version
# In this project we only enabling C++ project (CXX). For C based project replace CXX with C or you can mix CXX C
project(${PROJ_NAME} VERSION "${SVR_VERSION_MAJOR}.${SVR_VERSION_MINOR}.${SVR_VERSION_PATCH}.0" LANGUAGES CXX)

#-----------------------------------------------------------------------
add_definitions(-DQT_NO_KEYWORDS)

# Using cmake builtin CheckIncludeFileCXX micro tp
include(CheckIncludeFileCXX)

# Set this variables since they are not used so cmake will not give you warning like
# CMake Warning:
#   Manually-specified variables were not used by the project:
#
#    CMAKE_C_COMPILER
#    QT_QMAKE_EXECUTABLE

set(ignoreMe "${CMAKE_C_COMPILER} ${QT_QMAKE_EXECUTABLE}")

#-----------------------------------------------------------------------
# Some User defined Variables and default values. Also visible in Qt Creator UI
option(DEFINE_LIST_SOURCES
    "Print source files" OFF)               # Disabled by default

option(DEFINE_SVR_TRACE_ENABLE
    "Enable Trace Calls" OFF)               # Disabled by default

# When enabled run: make doc
option(DEFINE_DOXYGEN_ENABLE
    "Enable Doxygen documentation" OFF)     # Disabled by default

option(DEFINE_DOXYGEN_INSTALL
    "Install Doxygen documentation" OFF)    # Disabled by default

option(DEFINE_STACK_PROTECTOR
    "Build with stact protector" OFF)       # Disabled by default

option(DEFINE_POPPE_STACK_TRACE
    "Enable Poppe Stack Trace" OFF)         # Disabled by default

option(DEFINE_POPPE_PERF_CONTING
    "Enable Poppe Performant Counters" OFF) # Disabled by default

if(DEFINE_DOXYGEN_INSTALL)
    set(DEFINE_DOXYGEN_ENABLE ON)
endif(DEFINE_DOXYGEN_INSTALL)

#-----------------------------------------------------------------------

if(UNIX)
    set(PATH_SEPARATOR         '/')
else(UNIX)
    set(PATH_SEPARATOR         '\\')
endif(UNIX)

if(LINUX)
    set(LINUX_SYSTEM 1)
else(LINUX)
    set(LINUX_SYSTEM 0)
endif(LINUX)

macro(list_files list)
    if (DEFINE_LIST_SOURCES )
        foreach(infileName ${list})
            message(STATUS " ${infileName}")
        endforeach(infileName)
    endif(DEFINE_LIST_SOURCES)
endmacro()

#-----------------------------------------------------------------------
# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS Off)       # Disable -std=gnu++11 and enable -std=c++11

# Always be pedantic about your code. Compilation warrnings should be treated as ERRORS!!!! Fix it...
set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -Wunused-parameter -Wformat-security -Wunreachable-code -fdata-sections -ffunction-sections -Wcast-align")
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wunused-parameter -Wformat-security -Wunreachable-code -fdata-sections -ffunction-sections -Wcast-align")

if(DEFINE_STACK_PROTECTOR)
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -fstack-protector-all")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -fstack-protector-all")
endif(DEFINE_STACK_PROTECTOR)

# If LANGUAGES C enable above in project()
set(CMAKE_C_FLAGS_DEBUG   "-O0" CACHE INTERNAL "C debug compiler flags")
# If LANGUAGES CXX enable above in project()
set(CMAKE_CXX_FLAGS_DEBUG "-O0" CACHE INTERNAL "C++ debug compiler flags")
# If ASM files included in project
set(CMAKE_ASM_FLAGS_DEBUG "-g"  CACHE INTERNAL "ASM debug compiler flags")

# If LANGUAGES C enable above in project()
set(CMAKE_C_FLAGS_RELEASE   "-Os -flto" CACHE INTERNAL "C release compiler flags")
# If LANGUAGES CXX enable above in project()
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto" CACHE INTERNAL "C++ release compiler flags")
# If ASM files included in project
set(CMAKE_ASM_FLAGS_RELEASE ""          CACHE INTERNAL "ASM release compiler flags")

#-----------------------------------------------------------------------
# If cmake -DCMAKE_BUILD_TYPE=[DEBUG|RELEASE] ..
if( NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

string( TOUPPER ${CMAKE_BUILD_TYPE} DEFINE_DEBUG_BUILD)

# AB_BUILD_TYPE is used to flag source code DEBUG definition
if(DEFINE_DEBUG_BUILD STREQUAL DEBUG)
    set(DEFINE_DEBUG_BUILD "1")
    add_definitions(-DDEBUG)    # This is set to output in buildVersion.h.in
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
    endif()
else()
    set(DEFINE_DEBUG_BUILD "0") # This is set to output in buildVersion.h.in
    add_definitions(-DRELEASE)
endif()

#-----------------------------------------------------------------------
# Add user created cmake modules. Like FindGit in this case. This module
# replaces cmake own Git module which is missing some features
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)
    find_package(Git)
    if(GIT_FOUND)
        find_program(GIT_EXECUTABLE git
                     DOC "git command line client")

        message( STATUS "Building from GIT")
        Git_WC_INFO(${PROJECT_SOURCE_DIR} Project)
        message( STATUS "Git Tools : ${GIT_VERSION_STRING}")
        message( STATUS "Revision  : ${Project_WC_REVISION}")
        message( STATUS "Creator   : ${Project_WC_LAST_CHANGED_AUTHOR}")
        message( STATUS "Date      : ${Project_WC_LAST_CHANGED_DATE}")

        #Git_WC_LOG(${PROJECT_SOURCE_DIR} Project)
        #message( STATUS "Log:  ${Project_LAST_CHANGED_LOG}")

        set( SVR_VERSION_REVISION  ${Project_WC_REVISION})
    endif(GIT_FOUND)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.svn)
    find_package(Subversion)
    if(SUBVERSION_FOUND)
        find_program(SVN_EXECUTABLE svn
                     DOC "subversion command line client")

        message( STATUS "Building from SVN")

        Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
        message( STATUS "SVN tools : ${SVN_VERSION_STRING}")
        message( STATUS "Revision  : ${Project_WC_REVISION}")
        message( STATUS "Creator   : ${Project_WC_LAST_CHANGED_AUTHOR}")
        message( STATUS "Date      : ${Project_WC_LAST_CHANGED_DATE}")

        #Subversion_WC_LOG( ${PROJECT_SOURCE_DIR} Project)
        #message( STATUS "Log:  ${Project_LAST_CHANGED_LOG}")

        set( SVR_VERSION_REVISION  ${Project_WC_REVISION})
    endif(SUBVERSION_FOUND)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)

set(SVR_BUILD_VERSION "${SVR_VERSION_MAJOR}.${SVR_VERSION_MINOR}.${SVR_VERSION_PATCH}.${SVR_VERSION_REVISION}")

#-----------------------------------------------------------------------
if(DEFINE_DOXYGEN_ENABLE)
    # add a target to generate API documentation with Doxygen
    # If found you can run: make
    find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        if(APPLE)
            message(STATUS "brew install doxygen graphviz")
        else(UNIX)
            message(STATUS "sudo apt-get install doxygen-gui graphviz")
        endif()
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        message(FATAL_ERROR " You need to install doxygen utility")
    endif(NOT DOXYGEN_FOUND)
endif(DEFINE_DOXYGEN_ENABLE)

find_package(MySQL)

if(MYSQL_FOUND)
    include_directories(${MYSDQL_INCLUDE_DIR})
else(MYSQL_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    # https://downloads.mariadb.org/mariadb/repositories/#mirror=osuosl
    #
    if(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to cross compile libmariadb-dev first")
    else(CMAKE_CROSSCOMPILING)
        if(APPLE)
            message(STATUS "brew install libmariadb-dev (>= 10.2.8)")
        else()
            message(STATUS "sudo apt-get install libmariadb-dev (>= 10.2.8)")
        endif()
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        message(FATAL_ERROR " You need to install libmariadb-dev librarty")
    endif(CMAKE_CROSSCOMPILING)
endif(MYSQL_FOUND)

find_package(SPDLog 0.13.0)

if(SPDLOG_FOUND)
    include_directories(${SPDLOG_INCLUDE_DIR})
else(SPDLOG_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    # git clone --depth 1 -b v0.13.0 https://github.com/gabime/spdlog.git
    #
    if(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to cross compile libspdlog-dev first")
    else(CMAKE_CROSSCOMPILING)
        if(APPLE)
            message(STATUS "brew install spdlog (>= 0.13.0)")
        else()
            message(STATUS "sudo apt-get install libspdlog-dev (>= 0.13.0)")
        endif()
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        message(FATAL_ERROR " You need to install libspdlog-dev librarty")
    endif(CMAKE_CROSSCOMPILING)
endif(SPDLOG_FOUND)

find_package(Protobuf 3.3.2)

if(PROTOBUF_FOUND)
    include_directories(SYSTEM ${PROTOBUF_INCLUDE_DIRS})
else(PROTOBUF_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    if(CMAKE_CROSSCOMPILING)
        message(STATUS "You need to cross compile protobuf first")
        message(STATUS "git clone --depth 1 -b v3.3.2 https://github.com/google/protobuf.git")
    else(CMAKE_CROSSCOMPILING)
        message(STATUS "You need to compile protobuf first")
        message(STATUS "git clone --depth 1 -b v3.3.2 https://github.com/google/protobuf.git")
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        message(FATAL_ERROR " You need to install protobuf librarty")
    endif(CMAKE_CROSSCOMPILING)
endif(PROTOBUF_FOUND)

find_package(YamlCpp)

if(YAMLCPP_FOUND)
    include_directories(${YAMLCPP_INCLUDE_DIRS})
else(YAMLCPP_FOUND)
    message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    if(CMAKE_CROSSCOMPILING)
        message(FATAL_ERROR "You need to cross compile libyaml-cpp-dev first")
    else(CMAKE_CROSSCOMPILING)
        if(APPLE)
            message(STATUS "brew install yaml-cpp")
        else()
            message(STATUS "sudo apt-get install libyaml-cpp-dev")
        endif()
        message(STATUS "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        message(FATAL_ERROR " You need to install yaml-cpp librarty")
    endif(CMAKE_CROSSCOMPILING)
endif(YAMLCPP_FOUND)

if(DEFINE_POPPE_PERF_CONTING OR DEFINE_POPPE_STACK_TRACE)
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/include )
endif(DEFINE_POPPE_PERF_CONTING OR DEFINE_POPPE_STACK_TRACE)

# if cmake -DCMAKE_INSTALL_PREFIX=path ..
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/opt/autobon"
        CACHE PATH "${PROJ_NAME_IMAGE} install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#-----------------------------------------------------------------------
set(SVR_CONFIG_BIN_PATH    ${CMAKE_CURRENT_BINARY_DIR}/bin/${CROSS_COMPILE_ARCH})
set(SVR_CONFIG_SRC_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SVR_CONFIG_3RD_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)
set(SVR_CONFIG_CMAKE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(SVR_CONFIG_BIN_INCLUDE ${SVR_CONFIG_BIN_PATH}/include)

#-----------------------------------------------------------------------
message(STATUS "=========================================")

message(STATUS "  CMake Version: ${CMAKE_VERSION}")
message(STATUS "  Build Version  ${SVR_BUILD_VERSION}")
message(STATUS "  System Version ${CMAKE_SYSTEM_VERSION}")
message(STATUS "  Force RPATH Install=${DEFINE_INSTALL_RPATH_ENABLE}")
message(STATUS " -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS " -DDEFINE_STACK_PROTECTOR=${DEFINE_STACK_PROTECTOR}")
message(STATUS " -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message(STATUS " -DDEFINE_LIST_SOURCES=${DEFINE_LIST_SOURCES}")
message(STATUS " -DDEFINE_DOXYGEN_ENABLE=${DEFINE_DOXYGEN_ENABLE}")
message(STATUS " -DDEFINE_DOXYGEN_INSTALL=${DEFINE_DOXYGEN_INSTALL}")
message(STATUS " -DDEFINE_POPPE_STACK_TRACE=${DEFINE_POPPE_STACK_TRACE}")
message(STATUS " -DDEFINE_POPPE_PERF_CONTING=${DEFINE_POPPE_PERF_CONTING}")

message(STATUS "=========================================")

#-----------------------------------------------------------------------
# Remap above values which will be used to generate header file
if(DEFINE_SVR_TRACE_ENABLE)
    set(DEFINE_SVR_TRACE_ENABLE 1)
else()
    set(DEFINE_SVR_TRACE_ENABLE 0)
endif()

if(DEFINE_POPPE_STACK_TRACE)
    set(DEFINE_POPPE_STACK_TRACE 1)
else()
    set(DEFINE_POPPE_STACK_TRACE 0)
endif()

if(DEFINE_POPPE_PERF_CONTING)
    set(DEFINE_POPPE_PERF_CONTING 1)
else()
    set(DEFINE_POPPE_PERF_CONTING 0)
endif()

#-----------------------------------------------------------------------
# process any "in" files
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/abAppTools.h.in
    ${SVR_CONFIG_SRC_PATH}/include/abAppTools.h @ONLY)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/abAppToolsBuildType.h.in
    ${SVR_CONFIG_SRC_PATH}/include/abAppToolsBuildType.h @ONLY)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/abAppToolsBuildInfo.txt.in
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/abAppToolsBuildInfo.txt
    IMMEDIATE @ONLY)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/abGameServer.sh.in
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/abGameServer.sh
    IMMEDIATE @ONLY)

#-----------------------------------------------------------------------
# These section enables QT Creator support
FILE(GLOB_RECURSE InFiles "${CMAKE_CURRENT_SOURCE_DIR}/cmake/inConfig/*.in")

add_custom_target(in-files SOURCES ${InFiles})

FILE(GLOB_RECURSE moduleFiles "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/*.cmake"
                              "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.cmake")

add_custom_target(modules SOURCES ${moduleFiles})

FILE(GLOB_RECURSE ReadmeFiles "${SVR_CONFIG_3RD_PATH}/readme/*"
                              "${CMAKE_CURRENT_SOURCE_DIR}/cmake/README.txt"
                              "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Copyright.txt")

add_custom_target(readme SOURCES ${ReadmeFiles})

FILE(GLOB_RECURSE protobFiles "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/*.proto")

add_custom_target(protobuf SOURCES ${protobFiles})

FILE(GLOB_RECURSE includeFiles "${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h")

add_custom_target(include SOURCES ${includeFiles})

#-----------------------------------------------------------------------
set(BUILD_SHARED_LIBS ON)

if(NOT BUILD_SHARED_LIBS)
    message(STATUS "Building static library")
    # We will static link with external libs
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "-static -ldl -lc")
else()
    message(STATUS "Building shared library")
endif(NOT BUILD_SHARED_LIBS)

include(cmake/IncludeAppLib.cmake)

message(STATUS "Building ${COMPANY_NAME} - Uninstaller")

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/inConfig/cmake_uninstall.cmake)

if(DOXYGEN_FOUND)
    message(STATUS "Building ${COMPANY_NAME} - Doxygen documentation")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)

    # Remove ALL if you do not want to auto create
    # doxygen on make. If you remove ALL then you will
    # have to do "make doxygen"
    #add_custom_target(doxygen ALL

    if(DEFINE_DOXYGEN_INSTALL)
        set(BUILD_ON_MAKE ALL)
    endif(DEFINE_DOXYGEN_INSTALL)

    add_custom_target(doxygen ${BUILD_ON_MAKE}
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)

endif(DOXYGEN_FOUND)

if(DEFINE_DOXYGEN_ENABLE AND DEFINE_DOXYGEN_INSTALL)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen-html/ DESTINATION share/${COMPANY_NAME}/${PROJ_NAME})
endif(DEFINE_DOXYGEN_ENABLE AND DEFINE_DOXYGEN_INSTALL)

#-----------------------------------------------------------------------

set(SRC src)

#set output folder there potobuf generates *.pb.h output file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${SRC}/protoLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SRC}/include)

# Targets to be build
message(STATUS "=====================================================")

add_subdirectory(${SRC}/appLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/dbLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/udpLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/tcpLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/sslLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/protoLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/tictoeLogicLib/)

message(STATUS "=====================================================")

add_subdirectory(${SRC}/gameServer/)

#-----------------------------------------------------------------------
# CPack
include(InstallRequiredSystemLibraries)

set(CPACK_SET_DESTDIR true)
set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "${COMPANY_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Game Server. Target ${CMAKE_SYSTEM_VERSION}")

set(CPACK_PACKAGE_VERSION_MAJOR "${SVR_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${SVR_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${SVR_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${SVR_BUILD_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Copyright.txt")

set(CPACK_DEBIAN_FILE_NAME "${PROJ_NAME}-${SVR_BUILD_VERSION}-${CMAKE_SYSTEM_PROCESSOR}.deb")
set(CPACK_DEBIAN_PACKAGE_SECTION "admin")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "required")
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Robert J. Gebis")

set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)

include(CPack)
