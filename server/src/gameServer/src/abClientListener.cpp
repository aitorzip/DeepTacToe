#include "abClientListener.h"

ClientListener::ClientListener(ILogMessage& logger)
    : _logger(logger)
{
    _server.onStatusEvt.connect(this,     &ClientListener::onSocketStatus);
    _server.onConnectionEvt.connect(this, &ClientListener::onClientConnection);
}

ClientListener::~ClientListener(void)
{

}

void    ClientListener::initialize(const std::string& iFaceAndPort)
{
    _server.initialize(iFaceAndPort);

}

void    ClientListener::startAcceptingClients(void)
{
    _logger.onModuleMessage(ILogMessage::eSeverity::eInfo,
                            "ClientListener Accepting Clients");

    _server.spinServer();
}

void    ClientListener::stopAcceptingClients(void)
{
    _logger.onModuleMessage(ILogMessage::eSeverity::eInfo,
                            "ClientListener Stoping Accepting Clients");
}
