# This module defines
# SPDLOG_FOUND, if false, do not try to link to SPDLOG
# SPDLOG_INCLUDE_DIR, where to find spdlog.h
#
message(STATUS "Looking for spdlog")

if (SPDLOG_INCLUDE_DIR)
  # in cache already
  set(SPDLOG_FOUND TRUE)
else (SPDLOG_INCLUDE_DIR)
    find_path(  SPDLOG_INCLUDE_DIR
                NAMES spdlog/spdlog.h
                HINTS $ENV{SPDLOGDIR}
                PATHS /usr/local/include /usr/include)

    if (SPDLOG_INCLUDE_DIR)
        set(SPDLOG_FOUND TRUE)
    endif (SPDLOG_INCLUDE_DIR)

    if (NOT SPDLOG_FOUND)
        if (SPDLOG_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find spdlog")
        endif (SPDLOG_FIND_REQUIRED)
    endif (NOT SPDLOG_FOUND)

    # show the SPDLOG_INCLUDE_DIR variables only in the advanced view
    mark_as_advanced(SPDLOG_INCLUDE_DIR)

endif (SPDLOG_INCLUDE_DIR)


if(SPDLOG_FOUND)
    message(STATUS "Looking for spdlog - found")
    if (LIBFIND_DEBUG)
      message(STATUS "  SPDLOG_INCLUDE_DIR=${SPDLOG_INCLUDE_DIR}")
    endif(LIBFIND_DEBUG)
endif(SPDLOG_FOUND)
