cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "Game Server Application")

set(GAME_SERVER_NAME abGameServer)
 
message(STATUS "= Building ${DESCRIPTION}")

# Include *.h files to VS or Qt IDE can edit
file(GLOB game_svr_SRCS src/*.cpp include/*.h)

list_files( "${game_svr_SRCS}" )

include_directories(include
                    ${APP_CONFIG_SRC_PATH}) # include buildVersion.h

# add the executable
add_executable(${GAME_SERVER_NAME} ${game_svr_SRCS})

target_link_libraries(${GAME_SERVER_NAME} abProtolib abTcplib abApplib ${YAMLCPP_LIBRARIES} pthread )

set_target_properties( ${GAME_SERVER_NAME}
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SVR_CONFIG_BIN_PATH} )

install (TARGETS ${GAME_SERVER_NAME} DESTINATION bin)

