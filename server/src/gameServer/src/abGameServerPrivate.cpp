#include "abGameServer.h"
#include "abPathUtil.h"
#include "abStringUtil.h"
#include "abServerConfig.h"
#include <csignal>

bool GameServer::onInitialize(void)
{
    if( findArg("-gen"))
    {
        std::cout << ServerConfig::genTemplate() << std::endl;
        return false;
    }

    string      yamlFile;

    if( findArg("-yaml", yamlFile))
    {
        if(yamlFile.empty())
        {
            onErrorMessage("Missing -yaml parameter");
            return false;
        }

        try {
            _serverConfig.loadFromFile(yamlFile);
        }
        catch(std::exception& ex)
        {
            onErrorMessage(ex.what());
            return false;
        }
    }
    else
    {
        onErrorMessage("Missing -yaml switch");
        return false;
    }

    _clientListener.initialize(_serverConfig.iFaceAndPort());

    return true;
}

void GameServer::onCreate(void)
{

}

void GameServer::onStart(void)
{
    _clientListener.startAcceptingClients();
}

void GameServer::onResume(void)
{

}

void GameServer::onRunning(void)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void GameServer::onPause(void)
{

}

void GameServer::onStop(void)
{
    _clientListener.stopAcceptingClients();
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

    requiredList.push_back(make_pair("-yaml file", "use configuration from file"));
    optionalList.push_back(make_pair("-gen",       "generate yaml template"));
}

bool GameServer::setupFromYaml(void)
{

    return true;
}

bool GameServer::setupFromCommand(void)
{

    return true;
}

