#ifndef AB_IAPPLICATION_H
#define AB_IAPPLICATION_H

// IApplication - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abTypeDef.h"
#include "abAppTools.h"
#include "abMacros.h"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <sstream>
#include <mutex>

extern "C" { void abSigHandler(int); }

namespace spd = spdlog;

#define AB_PRINT_HEX  "-print-hex"

class IApplication
{
    friend void abSigHandler(int);

    public:
        enum class eDaemonize
        {
            No,
            Yes
        };

        typedef lock_guard<mutex>               TLockGuard;
        typedef std::shared_ptr<spdlog::logger> TLogger;

        enum eRunState
        {
            eInitial = 0,
            eCreating,
            eStarting,
            eResumeing,
            eRunning,
            ePausing,
            eStoping,
            eRestarting,
            eTerminating
        };

                 IApplication(eDaemonize type = eDaemonize::Yes);
        virtual ~IApplication(void);

        // Upon successfull initialization onCreate will be called
        // passing parsed command line options
        void    initialize(int32_t argc, char* argv[]) noexcept(false);

        bool    isInitialized(void) const { return _runState != eInitial and
                                                   _runState != eTerminating; }

        void    printUsage(void);
        void    printUsage(std::ostream& ss);

        // If command line -d param passed spin will call daemonize else run()
        void    spin(void);

        void    requestPause(void);
        void    requestResume(void);
        void    requestRestart(void);
        void    requestTerminate(void);

        bool    daemonized(void) const { return _daemonized; }

        void    traceLevel(spd::level::level_enum level);

        void    logException(const string& message);

    protected:
        // Invoked by initialize(argc, argv)
        virtual bool    onInitialize(void){ return true; }
        virtual void    onCreate(void)    {}

        // Invoked by run()
        virtual void    onStart(void)     {}
        virtual void    onResume(void)    {}
        virtual void    onRunning(void)   {}
        virtual void    onPause(void)     {}
        virtual void    onStop(void)      {}
        virtual void    onRestart(void)   {}
        virtual void    onTerminate(void) {}

        virtual void    onAppSigHandler(int32_t sig);

        virtual void    onUsage(TStringList&,      // descList
                                TArgPairList&,     // requiredList
                                TArgPairList&) {}  // optionalList

        virtual void    onEnvVariables(TStringList&) {}

        void    onVerboseMessage(const string& message);
        void    onDebugMessage(const string& message);
        void    onInfoMessage(const string& message);
        void    onWarnMessage(const string& message);
        void    onErrorMessage(const string& message);

        const TArgVec& Args(void) const { return _argVec; }

        // Check if command line switch exist
        bool    findArg(const string& param) const;
        // Check if command line arg exist and return its param
        bool    findArg(const string& param, string& paramVal) const;
        // Check if command line arg exist and return its param
        bool    findArg(const string& param, int32_t& paramVal) const;

        inline eRunState   runState(void) const { return _runState; }

    private:
        void    onRunCreate(void);
        void    onRunStart(void);
        void    onRunResume(void);
        void    onRunRunning(void);
        void    onRunPause(void);
        void    onRunStop(void);
        void    onRunRestart(void); 
        void    onRunTerminate(void);

        void    registerSigHandlers(void) noexcept(false);

        // This call is a blocking call (main loop)
        void    run(void) noexcept(false);

        // Same as above run() but with daemonizing  
        void    daemonize(void) noexcept(false);
        void    removePidFile(void);

        // Return name of compiled binary name
        string  name(void);

        string  buildInfo(void);
        void    checkFSAccess(void);
        void    startLogger(bool forceConsoleHelp=false);
        void    insertDefaults(TArgPairList& argDefList);

        void    checkInvalidArgs(TArgPairList& argReqList,
                                 TArgPairList& argOptList);

    protected:
        TLogger     _logger;

    private:
        eDaemonize  _eDaemonize;
        eRunState   _runState;
        TArgVec     _argVec;
        string      _pidFile;   // set if -d command line argument useed
        bool        _daemonized;
};

#if APP_TRACE
    #define TRACE_APP_INFO(m) onInfoMessage()
#else
    #define TRACE_APP_INFO(m)
#endif // APP_TRACE

#endif // AB_IAPPLICATION_H

