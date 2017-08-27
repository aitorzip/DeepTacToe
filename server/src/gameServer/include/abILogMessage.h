#ifndef AB_I_LOG_MESSAGE_H
#define AB_I_LOG_MESSAGE_H

// GameServer - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <string>

class ILogMessage
{
    public:
        virtual ~ILogMessage(void) {}

        enum class eSeverity
        {
            eVerbose,
            eDebug,
            eInfo,
            eWarning,
            eError,
            eCritical
        };

        virtual void onModuleMessage(eSeverity severity,
                                     const std::string& msg) = 0;
};

#endif // AB_I_LOG_MESSAGE_H
