#include "abGameServer.h"
#include "abPathUtil.h"
#include "abStringUtil.h"
#include <csignal>

bool GameServer::onInitialize(void)
{
    string genParam;

    if( findArg("-gen", genParam))
    {
        return false;
    }

    return true;
}

void GameServer::onCreate(void)
{

}

void GameServer::onStart(void)
{

}

void GameServer::onResume(void)
{

}

void GameServer::onRunning(void)
{

}

void GameServer::onPause(void)
{

}

void GameServer::onStop(void)
{

}

void GameServer::onRestart(void)
{
}

void GameServer::onTerminate(void)
{

}

void GameServer::onAppSigHandler(int32_t sig)
{

    IApplication::onAppSigHandler(sig);
}

void GameServer::onModuleMessage(eSeverity severity,
                                 const string &msg)
{
    switch(severity)
    {
        case eSeverity::eVerbose:
            onVerboseMessage(msg); break;

        case eSeverity::eDebug:
            onDebugMessage(msg); break;

        case eSeverity::eInfo:
            onInfoMessage(msg);  break;

        case eSeverity::eWarning:
            onWarnMessage(msg);  break;

        case eSeverity::eError:
            onErrorMessage(msg); break;

        case eSeverity::eCritical:
            logException(msg);   break;
    }
}

void GameServer::onUsage(TStringList&  descList,
                         TArgPairList& requiredList,
                         TArgPairList& optionalList)
{
    descList.push_back("Game Server");


    requiredList.push_back(make_pair("-l iface:port", "Listen for incomming connections"));

    optionalList.push_back(make_pair("-gen",       "generate yaml template"));
    optionalList.push_back(make_pair("-yaml file", "use configuration from file"));
}

bool GameServer::setupFromYaml(void)
{

    return true;
}

bool GameServer::setupFromCommand(void)
{

    return true;
}

