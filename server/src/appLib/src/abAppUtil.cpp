#include "abAppTools.h"  // Must be very first file
#include "abAppUtil.h"
#include "abPathUtil.h"
#include "abAppToolsBuildType.h"
#include <fstream>
#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <signal.h>

void AppUtil::checkArgLen(const TArgPairList& list)
{
    STACK

    for(auto item : list)
    {
        TEST_ASSERT(item.first.length() > AB_MAX_ARG_LEN, item.first + " is longer then " + std::to_string(AB_MAX_ARG_LEN));
    }
}

string   AppUtil::pidFile(const string& path,
                          const string& appName)
{
    STACK

    string appname(appName);

    string name = PathUtil::getSystemPath(path);
           name += appname;
           name += ".pid";

    return name;
}

bool AppUtil::pidFileExists(const string& path,
                            const string& appName)
{
   STACK

   return readPID(pidFile(path, appName)) != 0;
}

void AppUtil::logTempError(const string& name,
                           const string& message)
{
    STACK

    string path;

#ifdef WIN32
    path = "C:\\Windows\\temp\\";
#else
    path = "/tmp/";
#endif

    path += name;
    path += ".err";

    ofstream  stream;

    stream.open (path, std::ofstream::out | std::ofstream::app);

    if( stream.is_open() )
        stream << message << endl;
    else
        cerr << message << endl;
}

int32_t  AppUtil::readPID(const string& pidFile)
{
    STACK

    FILE *f;

    if (!(f=fopen(pidFile.c_str(),"r")))
        return 0;

    int32_t pid;

    if( fscanf(f ,"%d", &pid) == EOF)
    {
        fclose(f);
        return -1;
    }

    fclose(f);

    return pid;
}

int32_t  AppUtil::createPID(const string& pidFile)
{
    STACK

    FILE *f;
    int fd;
    int pid;

    if ( ((fd = open(pidFile.c_str(), O_RDWR | O_CREAT, 0644)) == -1)
         || ((f = fdopen(fd, "r+")) == NULL) )
        throw std::runtime_error("Can't open or create " + pidFile +
                                 ", Reason: " + std::strerror(errno) + ERROR_LOCATION);

#if HAVE_FLOCK
    if (flock(fd, LOCK_EX|LOCK_NB) == -1)
    {
        int e = errno;

        fscanf(f, "%d", &pid);

        fclose(f);
        throw std::runtime_error("Can't lock, lock is held by pid " + std::to_string(pid) +
                                 ", Reason: " + std::strerror(e) + ERROR_LOCATION);
    }
#endif

    pid = getpid();

    if (!fprintf(f,"%d\n", pid))
    {
        string error = string("Can't write pid to ") + pidFile +
                              ", Reason: " + std::strerror(errno) + ERROR_LOCATION;

        close(fd);

        throw std::runtime_error(error);
    }

    fflush(f);

#if HAVE_FLOCK
    if (flock(fd, LOCK_UN) == -1)
    {
        string error = "Can't unlock pidfile ";
               error += pidFile;
               error += ", Reason: ";
               error += std::strerror(errno);
               error += ERROR_LOCATION;

        close(fd);

        throw std::runtime_error(error);
    }
#endif

    close(fd);

    return pid;
}

int32_t  AppUtil::checkPID(const string& pidFile)
{
    STACK

    int32_t pid = readPID(pidFile);

    if ((!pid) || (pid == getpid()))
        return 0;

    if (kill(pid, 0) && errno == ESRCH)
        return(0);

    return pid;
}

int32_t  AppUtil::removePID(const string& pidFile)
{
    STACK

    return unlink(pidFile.c_str());
}

bool AppUtil::getEnvBool(const string& envName, bool& value)
{
    string envStr;

    if(getEnvString(envName, envStr) == false)
        return false;

    transform(envStr.begin(),
              envStr.end(),
              envStr.begin(),
              ::tolower);

    if( envStr == "true" )
    {
        value = true;
        return true;
    }
    else if( envStr == "false" )
    {
        value = false;
        return true;
    }
    else
        return false;
}

bool AppUtil::getEnvString(const string& envName, string& value)
{
    const char* pEnv = std::getenv(envName.c_str());

    if( pEnv )
    {
        value = pEnv;
        return true;
    }
    else
    {
        value.clear();
        return false;
    }
}
