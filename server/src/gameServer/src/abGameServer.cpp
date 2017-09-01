#include "abGameServer.h"
#include <iostream>

GameServer::GameServer()
 : IApplication(eDaemonize::No), _clientListener(*this),
   _activeClientMgr(*this)
{
    _clientListener.onNewUserEvt.connect(&_activeClientMgr, &ActiveClientMgr::onNewClient);
}

GameServer::~GameServer()
{

}
