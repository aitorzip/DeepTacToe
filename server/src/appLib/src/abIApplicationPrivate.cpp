#include "abAppTools.h"  // Must be very first file
#include "abIApplication.h"
#include "abAppUtil.h"
#include "abMacros.h"
#include "abPathUtil.h"
#include "abAppToolsBuildType.h"
#include <chrono>
#include <thread>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <sys/types.h>
#include <sys/stat.h>

#if LINUX_SYSTEM
    #include <sys/prctl.h>
#endif

const string PID_PATH="/var/run/autobon/";

spd::level::level_enum level(string logLevel)
{
    transform(logLevel.begin(), logLevel.end(), logLevel.begin(), ::tolower);

    if(logLevel.empty())
        return spd::level::info;

    int log = 0;

    for( auto item : spd::level::level_names)
    {
        if( logLevel == item )
            return static_cast<spd::level::level_enum>(log);

        ++log;
    }

    return spd::level::info;
}

void    IApplication::run(void) noexcept(false)
{
    STACK

    if( _runState != eStarting )
        throw std::runtime_error("Calling run() before initialize()?" +
                                 ERROR_LOCATION);

    bool    runLoop(true);

    while(runLoop)
    {
        switch(_runState)
        {
            case eStarting:
                // Call onStart() and move _runState to eResumeing
                onRunStart();
                break;

            case eResumeing:
                // Call onResume() and move _runState to eRunning
                onRunResume();
                break;

            case eRunning:
                onRunRunning();
                break;

            case ePausing:
                onRunPause();
                break;

            case eStoping:
                onRunStop();
                break;

            case eTerminating:
            default:
                runLoop = false;
                onRunTerminate();
                break;
        }
    }
}

void    IApplication::daemonize(void) noexcept(false)
{
    STACK

    _daemonized = true;

#ifndef _WIN32
    pid_t pid = fork();

    if (pid < 0)
         throw std::runtime_error(string("Failed to fork() process, Reason: ") +
                                  std::strerror(errno) +  ERROR_LOCATION);
    else if (pid != 0)
       _exit(0);   // parent terminates

    // child continues...

    // Become session leader
    setsid(); 

    // Clear our file mode creation mask
    umask(0); 

    // redirect stdin, stdout, and stderr to /dev/null
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    //signal(SIGHUP, SIG_IGN);
    //signal(SIGABRT, SIG_IGN);

    _logger->info("PID: " + std::to_string(getpid()));

    AppUtil::createPID(AppUtil::pidFile(PID_PATH, name()));

#endif

#if LINUX_SYSTEM
    prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);
#endif // LINUX_SYSTEM

    run();
}

void    IApplication::removePidFile(void)
{
    if(_daemonized)
        AppUtil::removePID(AppUtil::pidFile(PID_PATH, name()));
}

void    IApplication::checkFSAccess(void)
{
    STACK

    if( findArg("-d") )
    {
        string path = PathUtil::getSystemPath(PID_PATH);

        struct stat status;

        if( stat(path.c_str(), &status) != 0)   // Check Folder
            throw runtime_error("Error: " + path +
                                ", Reason: " + std::strerror(errno) + ERROR_LOCATION);

        if(S_ISDIR(status.st_mode) == false)
            throw runtime_error("Error: " + path + ", Reason: Is not folder" + ERROR_LOCATION);

        if((status.st_mode & S_IWUSR) != S_IWUSR)
            throw runtime_error("Error: " + path + ", Reason: You have not write permissions" + ERROR_LOCATION);

        if( AppUtil::pidFileExists(PID_PATH, name()) )
            throw runtime_error("Error: " + path + ", Reason: " + AppUtil::pidFile(PID_PATH, name()) +
                                                   " already exists" + ERROR_LOCATION);
    }
}

void    IApplication::startLogger(bool forceConsoleHelp)
{
    STACK

    string logPath;

    if( (forceConsoleHelp == false) && findArg("-f", logPath) )
    {
        if(logPath.empty())
            throw runtime_error("Missing -f parameter" + ERROR_LOCATION);

        auto path = PathUtil::getPath(logPath);

        if( path.length() )
        {
            struct stat info;

            if( stat( path.c_str(), &info ) != 0 )
            {
                if( mkdir(path.c_str(), S_IRWXU) == -1)
                    throw runtime_error( "Failed to create: " + path +
                                         ", Reason: " + std::strerror(errno) + ERROR_LOCATION);
            }
            else if( ! (info.st_mode & S_IFDIR) )
                throw runtime_error( "Please spcify path: " + path + ERROR_LOCATION);
        }

        string appName = name();

        transform(appName.begin(), appName.end(), appName.begin(), ::tolower);

        logPath = PathUtil::getSystemPath(path) + appName;

        try
        {
            _logger = spd::daily_logger_mt("file", logPath, 2, 30);
        }
        catch (const spd::spdlog_ex& ex)
        {
           throw runtime_error(string(ex.what()) + ERROR_LOCATION);
        }
    }
    else
    {
        try
        {
            _logger = spd::stdout_logger_mt("console");
        }
        catch (const spd::spdlog_ex& ex)
        {
           throw runtime_error(string(ex.what()) + ERROR_LOCATION);
        }
    }

    if( forceConsoleHelp )
        spd::set_pattern("%v");
    else
    {
        spd::set_pattern("[%H:%M:%S.%e] (%L) T(%t) %v");
        _logger->info("Log started");
    }

    _logger->info(string("\n")+ buildInfo());

    if( forceConsoleHelp == false )
    {
        const char* logLevel = std::getenv("AB_LOG_LEVEL");

        if( logLevel )
        {
            _logger->warn("Setting log level from AB_LOG_LEVEL");

            traceLevel( level( logLevel) );

            TEST_ASSERT(_logger->level() != level( logLevel), "Invalid $ENV(AB_LOG_LEVEL)");
        }
    }

    _logger->flush();
}

void    IApplication::onRunCreate(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onCreate");

    _runState = eCreating;

    onCreate();

    if(_runState != eTerminating)
        _runState = eStarting;
}

void    IApplication::onRunStart(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onStart");

    onStart();

    _runState = eResumeing;
}

void    IApplication::onRunResume(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onResume");

    onResume();

    _runState = eRunning;
}

void    IApplication::onRunRunning(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onRunning");

    onRunning();
}

void    IApplication::onRunPause(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onPause");

    _runState = ePausing;

    onPause(); 
}

void    IApplication::onRunStop(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onStop");

    _runState = eStoping;

    onStop();
}

void    IApplication::onRunRestart(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onRestart");

    _runState = eRestarting;

    onRestart();
}

void    IApplication::onRunTerminate(void)
{
    STACK

    TRACE_APP_INFO("IApplication::onTerminate");

    _runState = eTerminating;

    onTerminate();

    _logger->warn("Logger Closed");
}


bool    IApplication::findArg(const string& param ) const
{
    STACK

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter != _argVec.end() )
        return true;
    
    return false;
}

bool    IApplication::findArg(const string& param, string& paramVal) const
{
    STACK

    paramVal.clear();

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter == _argVec.end() )
        return false;

    ++iter;

    // Check if there is another item in list (paramVal)
    if( iter != _argVec.end() )
    {
        if( iter->empty() == false)
        {
            if( (*iter)[0] != '-' )
            {
                paramVal = *iter;

                if(paramVal.empty())
                    throw std::runtime_error("Failed to parse " + param +
                                             " parameter: empty" +
                                             ERROR_LOCATION);
            }
        }
    }

    return true;
}

bool    IApplication::findArg(const string& param, int32_t& paramVal) const
{
    STACK

    paramVal = 0;

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter == _argVec.end() )
        return false;

    ++iter;

    // Check if there is another item in list (paramVal)
    if( iter != _argVec.end() )
    {
        if( iter->empty() == false )
        {
            if( (*iter)[0] != '-' )
            {
                istringstream  a2i(*iter);

                if( ! (a2i >> paramVal) )
                    throw std::runtime_error("Failed to parse " + param +
                                             " parameter: " +
                                             *iter + ERROR_LOCATION);
            }
        }
    }

    return true;
}

string IApplication::name(void)
{
    STACK

    TEST_ASSERT(_argVec.empty(), "_argVec is empty");

    string path = _argVec.front();

    auto   pos  = path.find_last_of(PATH_SEPARATOR) + 1;

    return path.substr(pos, path.length() - pos );
}

extern IApplication* gABSigApp;

void   IApplication::onAppSigHandler(int32_t sig)
{
    STACK

    switch(sig)
    {
        case SIGHUP:
            // controlling terminal closed, Ctrl-D
            break;

        case SIGINT:
            // Ctrl-C
            gABSigApp->requestTerminate();
            break;

        case SIGQUIT:
            // Ctrl-\, clean quit with core dump
            break;

        case SIGTERM:
            //  kill command
            break;

        case SIGWINCH:
            //  terminal resize (ncurses?)
            break;

        case SIGABRT:
            // abort() called
            break;

        case SIGBUS:

            break;

        case SIGFPE:

            break;

        case SIGSEGV:
            break;

        case SIGPIPE:
            break;

        case SIGUSR1:
            {
                spdlog::level::level_enum level = _logger->level();

                if( level == spdlog::level::trace)
                {
                    _logger->flush();
                    return;
                }

                level = static_cast<spdlog::level::level_enum>( static_cast<int>(level) - 1 );

                traceLevel(level);
            }
            break;

        case SIGUSR2:
            {
                spdlog::level::level_enum level = _logger->level();

                if( level == spdlog::level::warn)
                {
                    _logger->flush();
                    return;
                }

                level = static_cast<spdlog::level::level_enum>( static_cast<int>(level) + 1 );

                traceLevel(level);
            }
            break;

#if LINUX_SYSTEM
        case SIGSTKFLT:
            break;
#endif // LINUX_SYSTEM
        //defaul:
        //  break;
    }
}

