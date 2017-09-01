#ifndef AB_CLIENT_LISTENER_H
#define AB_CLIENT_LISTENER_H

// ClientListener - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abActiveClientBase.h"

class ClientListener
{
    public:
        typedef Signal<ActiveClientBase::TClientPtr&> TNewUserEvent;

                 ClientListener(ILogMessage& logger);
        virtual ~ClientListener(void);

        // TCP initialization
        // hostAndPort - iface:port
        void    initialize(const std::string& iFaceAndPort);

        void    startAcceptingClients(void);
        void    stopAcceptingClients(void);

        TNewUserEvent   onNewUserEvt;

    private:
        void onSocketStatus(const string           msg,
                            const ISocket::eStatus status);

        void onClientConnection(TCPSocket* socket);

        void deserializeClientMessage(const int8_t* pBuffer,
                                      uint32_t      msgSize);

    private:
        ILogMessage&    _logger;
        TCPSocket       _server;
};

#endif // AB_CLIENT_LISTENER_H
