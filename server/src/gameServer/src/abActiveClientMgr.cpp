#include "abActiveClientMgr.h"

ActiveClientMgr::ActiveClientMgr(ILogMessage& logger)
    : _logger(logger)
{

}

ActiveClientMgr::~ActiveClientMgr(void)
{

}

void    ActiveClientMgr::startAcceptingClients(void)
{
    _logger.onModuleMessage(ILogMessage::eSeverity::eInfo,
                            " ");
}

void    ActiveClientMgr::stopAcceptingClients(void)
{

}
