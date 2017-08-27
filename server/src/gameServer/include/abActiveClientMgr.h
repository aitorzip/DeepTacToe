#ifndef AB_ACTIVE_CLIENT_MGR_H
#define AB_ACTIVE_CLIENT_MGR_H

// ActiveClientMgr - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abTcpSocket.h"
#include "abILogMessage.h"

class ActiveClientMgr
{
    public:
                 ActiveClientMgr(ILogMessage& logger);
        virtual ~ActiveClientMgr(void);

        void    startAcceptingClients(void);
        void    stopAcceptingClients(void);

    private:
        ILogMessage&    _logger;
        TCPSocket       _server;
};

#endif //
