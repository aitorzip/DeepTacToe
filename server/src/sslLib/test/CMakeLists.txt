cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Autobon.ai - SSL Library Test")

set(AB_SSL_TEST_NAME        abTestSslLib)

message(STATUS "= Building ${DESCRIPTION}")

include_directories(include
                    ../../include
                    ${CXXTEST_INCLUDE_DIR}
                    ${CAN_CONFIG_BIN_INCLUDE})

# You can have multiple cxxtest_add_test
# One for each *.h file
cxxtest_add_test(${AB_SSL_TEST_NAME}
                 cxxSslLibTestHarnes.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/cxxSslLibTestHarnes.h)

# Include *.h files to VS or Qt IDE can edit
file(GLOB ssl_test_SRCS src/*.cpp
                          include/*.h)

list_files( "${ssl_test_SRCS}" )

target_link_libraries(${AB_SSL_TEST_NAME} abSsllib abTcplib pthread)

set_target_properties( ${AB_SSL_TEST_NAME}
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CAN_CONFIG_BIN_PATH} )

if(NOT DEFINE_BUILD_SDK)
    install (TARGETS ${AB_SSL_TEST_NAME} DESTINATION bin)
endif(NOT DEFINE_BUILD_SDK)

add_test(NAME "abSSLLibTest"
        COMMAND ${AB_SSL_TEST_NAME})
