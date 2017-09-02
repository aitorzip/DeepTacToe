#include "abClientListener.h"
#include "abReadBuffer.h"
#include "abProto.h"

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
    static thread_local ReadBuffer readBuffer;

    uint32_t    msgSize(0);
    int32_t     ret;

    try
    {
        ret = socket->receive(msgSize);

        if( ret <= 0 and msgSize < 2)
        {
            _logger.onModuleMessage(ILogMessage::eSeverity::eError, "ClientListener Failed To Read Msg Size: " + std::to_string(ret));
            delete socket;
            return;
        }

        readBuffer.realloc(msgSize);

        ret = socket->receive(readBuffer.buffer(), msgSize);

        if( ret <= 0 )
        {
            _logger.onModuleMessage(ILogMessage::eSeverity::eError, "ClientListener Failed To Read Msg: " + std::to_string(ret));
            delete socket;
            return;
        }

        deserializeClientMessage(socket, readBuffer.buffer(), msgSize);
    }
    catch(std::exception& ex)
    {
        _logger.onModuleMessage(ILogMessage::eSeverity::eError, string("ClientListener ") + ex.what());
        delete socket;
        return;
    }
}

void ClientListener::deserializeClientMessage(TCPSocket*    socket,
                                              const int8_t* pBuffer,
                                              uint32_t      msgSize)
{
    // msgSize - size of protobug message
    // NOT INCLUDING first two bytes which is message type

    GameServerPB::eGameMsgType type = GameServerPB::Serializer::msgType(pBuffer);

    if( type == GameServerPB::eUnknown)
        throw std::runtime_error("invalid client msg type: " +
                       std::to_string(*reinterpret_cast<const uint16_t*>(pBuffer)));

    const int8_t* pPBuffer = pBuffer + sizeof(uint16_t);

    msgSize -= sizeof(uint16_t);

    switch(type)
    {
        case GameServerPB::eLogin:
        {
            GameServerPB::login login;
            if( login.ParseFromArray( pPBuffer, msgSize) == false )
                throw std::runtime_error("Error Parsing Message: loginReq");

            process(socket, login);
        } break;

        default:
            throw std::runtime_error("Unknown Message");
    }
}

void ClientListener::process(TCPSocket* socket, GameServerPB::login& login)
{
    GameServerPB::login_ack login_ack;

    if(login.guid() == DeepTacToePB::TIC_TOE_GUID)
    {
        if(login.wire_ver() != DeepTacToePB::TIC_TOE_FORMAT)
            throw std::runtime_error(string("DeepTacToePB Invalid Wire Format: ") +
                                     std::to_string(login.wire_ver()) + " Current: " +
                                     std::to_string(DeepTacToePB::TIC_TOE_FORMAT));
    }
    else
        throw std::runtime_error(string("Unknown GUID: ") + login.guid());

    /// TODO: Finish creating and handling DeepTickToe Client
    delete socket;
}
