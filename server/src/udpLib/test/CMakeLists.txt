cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Autobon.ai - UDP Library Test")

set(AB_UDP_TEST_NAME        abTestUdpLib)

message(STATUS "= Building ${DESCRIPTION}")

include_directories(include
                    ../../include
                    ${CXXTEST_INCLUDE_DIR}
                    ${CAN_CONFIG_BIN_INCLUDE})

# You can have multiple cxxtest_add_test
# One for each *.h file
cxxtest_add_test(${AB_UDP_TEST_NAME}
                 cxxUdpLibTestHarnes.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/include/cxxUdpLibTestHarnes.h)

# Include *.h files to VS or Qt IDE can edit
file(GLOB udp_test_SRCS src/*.cpp
                        include/*.h)

list_files( "${udp_test_SRCS}" )

target_link_libraries(${AB_UDP_TEST_NAME} abTcplib pthread)

set_target_properties( ${AB_UDP_TEST_NAME}
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CAN_CONFIG_BIN_PATH} )

if(NOT DEFINE_BUILD_SDK)
    install (TARGETS ${AB_UDP_TEST_NAME} DESTINATION bin)
endif(NOT DEFINE_BUILD_SDK)

add_test(NAME "abUDPLibTest"
         COMMAND ${AB_UDP_TEST_NAME})
