#include "abClientListener.h"

void ClientListener::onSocketStatus(const string           msg,
                                    const ISocket::eStatus status)
{
    if( status == ISocket::eInfo )
        _logger.onModuleMessage(ILogMessage::eSeverity::eInfo, msg);
    else if( status == ISocket::eError )
        _logger.onModuleMessage(ILogMessage::eSeverity::eError, msg);
    else if( status == ISocket::eConnected )
        _logger.onModuleMessage(ILogMessage::eSeverity::eInfo, "ClientListener connected to: " + msg);
    else if( status == ISocket::eDisconnected )
        _logger.onModuleMessage(ILogMessage::eSeverity::eInfo, "ClientListener disconnected: " + msg);
    else if( status == ISocket::eListening )
        _logger.onModuleMessage(ILogMessage::eSeverity::eInfo, "ClientListener listening on: " + msg);
}

void ClientListener::onClientConnection(TCPSocket* socket)
{
    /// TODO: onClientConnection
    delete socket;
}
