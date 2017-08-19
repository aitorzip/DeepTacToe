# The version number is used if not .git folder is found.
message(STATUS "Including AppLib cmake")

set(APP_VERSION_MAJOR      0)
set(APP_VERSION_MINOR      1)
set(APP_VERSION_PATCH      1)
set(APP_VERSION_REVISION   ~)

message(STATUS "==============================================================")

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

        set( APP_VERSION_REVISION  ${Project_WC_REVISION})
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

        set( APP_VERSION_REVISION  ${Project_WC_REVISION})
    endif(SUBVERSION_FOUND)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git)

set(APP_BUILD_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_VERSION_REVISION}")
