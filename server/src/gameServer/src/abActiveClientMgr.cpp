#include "abActiveClientMgr.h"

ActiveClientMgr::ActiveClientMgr(ILogMessage& logger)
    : _logger(logger)
{

}

ActiveClientMgr::~ActiveClientMgr(void)
{

}

void    ActiveClientMgr::onNewClient(ActiveClientBase::TClientPtr& client)
{
    (void)client;
    /// TODO: Finish handing new user
}
