#include "abAppTools.h"  // Must be very first file
#include "abIApplication.h"
#include "abStringUtil.h"
#include "abAppUtil.h"
#include "abPathUtil.h"
#include "abAppToolsBuildType.h"
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SHOW_ENV_VARIABLES  "-print-env-var"

IApplication* gABSigApp(nullptr);

SIG_FUNCT_DEF_START(abSigHandler)
    STACK
    TEST_ASSERT(gABSigApp == nullptr, "gABSigApp is null");
    gABSigApp->onAppSigHandler(sig);
SIG_FUNCT_DEF_END()

IApplication::IApplication(eDaemonize type)
  : _eDaemonize(type), _runState(eInitial), _daemonized(false)
{
    STACK
    TEST_ASSERT(gABSigApp != nullptr, "Only one active IApplication instance allowed per process");

    gABSigApp = this;
}

IApplication::~IApplication(void)
{
    STACK
    gABSigApp = nullptr;

    removePidFile();

    spdlog::drop_all();
}

void    IApplication::initialize(int32_t argc, char* argv[]) noexcept(false)
{
    STACK
    _argVec.clear();

    for(int32_t i = 0; i < argc; i++)
        _argVec.push_back( argv[i] );

    if( _argVec.size() == 1 or findArg("-h") )
    {
        printUsage();
        return;
    }

    TStringList     desc;
    TArgPairList    argReqList;
    TArgPairList    argOptList;

    onUsage(desc, argReqList, argOptList);

    insertDefaults(argOptList);

    checkInvalidArgs(argReqList, argOptList);

    if( findArg(SHOW_ENV_VARIABLES) )
    {
        std::cout << std::endl << buildInfo() << std::endl << std::endl;

        TStringList list;

        list.push_back("export AB_LOG_LEVEL={trace|debug|info|warn}");

        onEnvVariables(list);

        std::cout << "--------------- ENV VARIABLES ---------------" << std::endl;

        for(auto item : list)
            std::cout << item << std::endl;

        std::cout << "---------------------------------------------" << std::endl;

        return;
    }

    checkFSAccess();

    startLogger();

    registerSigHandlers();

    if( _eDaemonize != eDaemonize::Yes and !findArg("-d"))
        _logger->info("PID: " + std::to_string(getpid()));

    if( onInitialize() == false )
        return;

    if(_runState != eTerminating)
        onRunCreate();
}

void    IApplication::traceLevel(spd::level::level_enum level)
{
    STACK

    _logger->warn(string("Log level changed to: ") + spdlog::level::to_str(level) );
    _logger->set_level(level);
    _logger->flush();
}

void    IApplication::spin()
{
    STACK

    if( _runState == eTerminating )
        return;

    if( _eDaemonize == eDaemonize::Yes and findArg("-d") )
        daemonize();
    else
        run();
}

void    IApplication::requestPause(void)
{
    STACK

    if( _runState == eTerminating )
        return;

    TRACE_APP_INFO("Pausing requested");

    _runState = ePausing;
}

void    IApplication::requestResume(void)
{
    STACK

    if( _runState == eTerminating )
        return;

    TRACE_APP_INFO("Resumeing requested");

    _runState = eResumeing;
}

void    IApplication::requestRestart(void)
{
    STACK

    if( _runState == eTerminating )
        return;

    TRACE_APP_INFO("Restart requested");

    _runState = eRestarting;
}

void    IApplication::requestTerminate(void)
{
    STACK

    if( _runState == eTerminating )
        return;

    TRACE_APP_INFO("Terminate requested");

    _runState = eTerminating;
}

string  IApplication::buildInfo(void)
{
    STACK

    string args;

    for(uint32_t idx(1); idx < _argVec.size(); ++idx)
        args.append(_argVec[idx] + " ");

    std::stringstream ss;

    // TODO: Replace APP_BUILD_VERSION with Build Version
    ss << "* Autobon.ai Copyright (c) 2016-" << (__DATE__ + 7) << endl;
    ss << "*  Ver  : " << APP_BUILD_VERSION << endl;
    ss << "*  Sys  : " << APP_BUILD_SYSTEM << endl;
    ss << "*  Date : " << __DATE__ << " @ " << __TIME__ << endl;
    ss << "*  Tool : " << COMPILER_VERSION << endl;
    //ss << "*  C++  : " << __cplusplus << endl;

    if(args.length())
        ss << "*  Arg  : " << args;

    return ss.str();
}
void    IApplication::onVerboseMessage(const string& message)
{
    STACK

    _logger->trace( message );
    _logger->flush();
}

void    IApplication::onDebugMessage(const string& message)
{
    STACK

    _logger->debug( message );
    _logger->flush();
}

void    IApplication::onInfoMessage(const string& message)
{
    STACK

    _logger->info( message );
    _logger->flush();
}

void    IApplication::onWarnMessage( const string& message)
{
    STACK

    _logger->warn( message );
    _logger->flush();
}

void    IApplication::onErrorMessage(const string& message)
{
    STACK

    _logger->error( message);
    _logger->flush();
}

void    IApplication::logException(const string& message)
{
    STACK

    _logger->critical(message);
    _logger->flush();
}

void    IApplication::printUsage(void)
{
    STACK

    std::stringstream ss;

    printUsage(ss);

    if( ! _logger )
        startLogger(true);

    _logger->info( ss.str() );
}

void    IApplication::printUsage(std::ostream& ss)
{
    STACK

    TStringList     desc;
    TArgPairList    argReqList;
    TArgPairList    argOptList;

    onUsage(desc, argReqList, argOptList);

    TEST_ARG_LEN(argReqList);
    TEST_ARG_LEN(argOptList);

    ss << endl;
    
    // Check for application description
    TEST_ASSERT(desc.empty(), "desc is empty");

    // Check for runtime command line parameters
    TEST_ASSERT(_argVec.empty(), "_argList is empty");
 
    TStringList::const_iterator iter = desc.begin();

    while(iter != desc.end())
    {
        if( iter == desc.begin() )
            ss << *iter << endl;
        else
            ss << "  " << *iter << endl;

        ++iter;
    }

    TArgPairList defParamList;

    insertDefaults(defParamList);

    ss << endl;
    ss << "Usage: " << name() << " args" << endl;

    for( auto item : defParamList)
        ss << "  " << StringUtil::padRight(item.first, AppUtil::AB_MAX_ARG_LEN)
           << " - " << item.second << endl;

    for( auto item : argReqList)
        ss << "! " << StringUtil::padRight(item.first, AppUtil::AB_MAX_ARG_LEN)
           << " - " << item.second << endl;

    for( auto item : argOptList)
        ss << "  " << StringUtil::padRight(item.first, AppUtil::AB_MAX_ARG_LEN)
           << " - " << item.second << endl;
}

void   IApplication::checkInvalidArgs(TArgPairList& argReqList,
                                      TArgPairList& argOptList)
{
    STACK

    for(uint32_t idx(1); idx < _argVec.size(); ++idx)
    {
        const string& arg = _argVec[idx];

        // Skip arg parameters
        if(arg[0] != '-')
            continue;

        for(auto pair : argReqList )
            if(pair.first.substr(0, arg.length()) == arg)
                goto next_step;

        for(auto pair : argOptList )
            if(pair.first.substr(0, arg.length()) == arg)
                goto next_step;

        if(SHOW_ENV_VARIABLES != arg)
            throw runtime_error("Unknown argument: " + arg +
                                ERROR_LOCATION);

        next_step:{}
    }
}

void   IApplication::insertDefaults(TArgPairList& argDefList)
{
    if(_eDaemonize == eDaemonize::Yes)
    {
        argDefList.push_front(make_pair("-sighup", "enable sighup. Valid if -d used"));
        argDefList.push_front(make_pair("-d", "run as daemon"));
    }

    argDefList.push_front(make_pair("-f path", "log path. Default screen"));
    argDefList.push_front(make_pair("-h", "print this help"));
}

void   IApplication::registerSigHandlers(void) noexcept(false)
{
    STACK

    // controlling terminal closed, Ctrl-D
    if( signal(SIGHUP, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGHUP. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    // Ctrl-C
    if( signal(SIGINT, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGINT. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    // Ctrl-\, clean quit with core dump
    if( signal(SIGQUIT, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGQUIT. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    // kill command
    if( signal(SIGTERM, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGTERM. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    // terminal window resize (ncurses?)
    if( signal(SIGWINCH, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGWINCH. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    // abort() called
    if( signal(SIGABRT, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGABRT. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    if( signal(SIGILL, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGILL. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    if( signal(SIGBUS, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGBUS. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    if( signal(SIGFPE, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGFPE. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    if( signal(SIGUSR1, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGUSR1. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    if( signal(SIGUSR2, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGUSR2. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    if( signal(SIGSEGV, abSigHandler) == SIG_ERR)
        throw runtime_error(string("Failed register SIGSEGV. ") +
                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);

    //if( signal(SIGPIPE, abSigHandler) == SIG_ERR)
    //    throw runtime_error(string("Failed register SIGPIPE. ") +
    //                        ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);
//#if LINUX_SYSTEM
//    if( signal(SIGSTKFLT, abSigHandler) == SIG_ERR)
//        throw runtime_error(string("Failed register SIGSTKFLT. ") +
//                            ", Reason: " + std::strerror(errno) +  ERROR_LOCATION);
//#endif // LINUX_SYSTEM

}

