#ifndef AB_ACTIVE_CLIENT_BASE_H
#define AB_ACTIVE_CLIENT_BASE_H

// ActiveClientBase - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abTcpSocket.h"
#include "abILogMessage.h"

class ActiveClientBase
{
    public:
        typedef std::unique_ptr<ActiveClientBase>  TClientPtr;

        enum class eType
        {
            eUnknown
        };

        ActiveClientBase(ILogMessage& logger, eType type);

        inline eType        type(void) const { return _type; }
               std::string  typeStr(void);

    private:
        ILogMessage& _logger;
        eType        _type;
};

#endif // AB_ACTIVE_CLIENT_BASE_H

