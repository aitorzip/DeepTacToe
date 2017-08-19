#  YAMLCPP_FOUND - system has yaml-cpp
#  YAMLCPP_INCLUDE_DIRS - the yaml-cpp include directory
#  YAMLCPP_LIBRARIES - Link these to use yaml-cpp
#  YAMLCPP_DEFINITIONS - Compiler switches required for using yaml-cpp

message(STATUS "Looking for yaml-cpp")

if (YAMLCPP_LIBRARIES AND YAMLCPP_INCLUDE_DIRS)
  # in cache already
  set(YAMLCPP_FOUND TRUE)
else (YAMLCPP_LIBRARIES AND YAMLCPP_INCLUDE_DIRS)
    find_path(  YAMLCPP_INCLUDE_DIR
                NAMES yaml.h
                PATHS /usr/local/include /usr/include
                PATH_SUFFIXES yaml-cpp)

    find_library( YAMLCPP_LIBRARY
                NAMES yaml-cpp
                PATHS /usr/local/lib /usr/lib )

    set(YAMLCPP_INCLUDE_DIRS ${YAMLCPP_INCLUDE_DIR})
    set(YAMLCPP_LIBRARIES ${YAMLCPP_LIBRARY} )

    if (YAMLCPP_INCLUDE_DIRS AND YAMLCPP_LIBRARIES)
        set(YAMLCPP_FOUND TRUE)
    endif (YAMLCPP_INCLUDE_DIRS AND YAMLCPP_LIBRARIES)

    if (NOT YAMLCPP_FOUND)
        if (YAMLCPP_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find yaml-cpp")
        endif (YAMLCPP_FIND_REQUIRED)
    endif (NOT YAMLCPP_FOUND)

    # show the YAMLCPP_INCLUDE_DIRS and YAMLCPP_LIBRARIES variables only in the advanced view
    mark_as_advanced(YAMLCPP_INCLUDE_DIRS YAMLCPP_LIBRARIES)

endif (YAMLCPP_LIBRARIES AND YAMLCPP_INCLUDE_DIRS)

if(YAMLCPP_FOUND)
    message(STATUS "Looking for yaml-cpp - found")
    if (LIBFIND_DEBUG)
        message(STATUS "  YAMLCPP_INCLUDE_DIR=${YAMLCPP_INCLUDE_DIR}")
        message(STATUS "  YAMLCPP_LIBRARY=${YAMLCPP_LIBRARY}")
        message(STATUS "  YAMLCPP_DEFINITIONS=${YAMLCPP_DEFINITIONS}")
    endif(LIBFIND_DEBUG)
endif(YAMLCPP_FOUND)
