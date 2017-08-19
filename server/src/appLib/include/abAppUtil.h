#ifndef AB_IAPP_UTIL_H
#define AB_IAPP_UTIL_H

// AppUtil - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abMacros.h"
#include "abTypeDef.h"

class AppUtil
{
    public:
        static const uint16_t AB_MAX_ARG_LEN = 24;

        static void checkArgLen(const TArgPairList& list);

        static string   pidFile(const string& path,
                                const string& appName);

        static bool     pidFileExists(const string& path,
                                      const string& appName);

        static bool     getEnvBool(const string& envName,   bool& value);
        static bool     getEnvString(const string& envName, string& value);

        static int32_t  readPID(const string& pidFile);
        static int32_t  createPID(const string& pidFile);
        static int32_t  checkPID(const string& pidFile);
        static int32_t  removePID(const string& pidFile);

        // This function is used to log message to /tmp/name.err
        static void     logTempError(const string& name,
                                     const string& message);
};

#if DEBUG
    #define TEST_ARG_LEN(list) {    \
        AppUtil::checkArgLen(list); \
    }
#else
    #define TEST_ARG_LEN(list) { }
#endif

#define SIG_FUNCT_DEF_START(func)   \
    extern "C" { void    func(int sig) {

#define SIG_FUNCT_DEF_END() }}

#endif // AB_IAPP_UTIL_H
 
