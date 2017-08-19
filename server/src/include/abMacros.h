#ifndef AB_MACROS_H
#define AB_MACROS_H

// ISocket - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abAppTools.h"
#include <cstring>

#ifdef WIN32 
    #ifdef EXPORT_DLL
        #define DllExport __declspec(dllexport)
    #else
        #define DllExport __declspec(dllimport)
    #endif

    #pragma warning(disable : 4251 4290 4514 4355)
    #define PACKED __attribute__((__packed__))
#else
    #define DllExport
    #define PACKED __attribute__((__packed__))
#endif

#if STACK_TRACING_ENABLED
    #define PRINT_STACK_TRACE(os)           \
        os << "--------- Stack---------\n"  \
           << Stack::GetTraceString() << std::endl;
#else
    #define PRINT_STACK_TRACE(os)
#endif

#define AB_UNUSED(x) (void)x;

#include <cassert>
#include <iostream>

#define TEST_ASSERT_PRIVATE(b, m, bv, bs) {                                                 \
    if( bool(b) ) {                                                         \
        std::string file(__FILE__);                                         \
        std::string::size_type pos = file.find_last_of(PATH_SEPARATOR) + 1; \
        file = file.substr(pos, file.length() - pos);                       \
        std::cout << "!! Guru Meditation !!! "           << std::endl       \
                  << "!! Build: " << bv << std::endl       \
                  << "!! Sys:   " << bs  << std::endl       \
                  << "!! At:    " << file                                   \
                  << " @ "        << __LINE__            << std::endl       \
                  << "!! Msg:   " << m                   << std::endl;      \
                  PRINT_STACK_TRACE(std::cout);                             \
        exit(-1);                                                           \
    }                                                                       \
}

#define APPEND_LAST_ERROR_STR( s )          \
{                                           \
    if( errno )                             \
    {                                       \
        s << " errno: ";                    \
        s << errno << ", Reason: ";         \
        s << std::strerror(errno);          \
        errno = 0;                          \
    }                                       \
}

#endif // AB_MACROS_H

