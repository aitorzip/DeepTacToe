#ifndef AB_APP_TOOLS_BUILD_TYPE_H
#define AB_APP_TOOLS_BUILD_TYPE_H

// Auto Generated Do NOT Edit!!!
// Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#define DEBUG                   1
#define APP_TRACE               
#define LINUX_SYSTEM            0

#define COMPILER_VERSION        __VERSION__

#define STACK_TRACING_ENABLED           0
#define PERFORMANCE_COUNTING_ENABLED    0

#if (STACK_TRACING_ENABLED or PERFORMANCE_COUNTING_ENABLED)
    #include "../../3rdParty/include/PoppyDebugTools.h"
#else
    #define STACK
    #define STACK_BLOCK(x)
    #define STACK_SECTION(x)
    #define STACK_VAL(var, value)
#endif

#ifdef DEBUG
    #define TEST_ASSERT(b, m) TEST_ASSERT_PRIVATE(b, m, APP_BUILD_VERSION, APP_BUILD_SYSTEM)
#else
    #define TEST_ASSERT(b, m) { }
#endif

#endif // AB_APP_TOOLS_BUILD_TYPE_H
