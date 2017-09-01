#ifndef AB_GAME_SERVER_H
#define AB_GAME_SERVER_H

// GameServer - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abIApplication.h"
#include "abILogMessage.h"
#include "abClientListener.h"
#include "abActiveClientMgr.h"
#include "abServerConfig.h"

class GameServer : public IApplication, public ILogMessage
{
    public:
         GameServer(void);
        ~GameServer(void) override;

    protected:
        // IApplication interfaces
        bool    onInitialize(void) override;
        void    onCreate(void) override;
        void    onStart(void) override;
        void    onResume(void) override;
        void    onRunning(void) override;
        void    onPause(void) override;
        void    onStop(void) override;
        void    onRestart(void) override;
        void    onTerminate(void) override;

        void    onAppSigHandler(int32_t sig) override;

        void    onUsage(TStringList&  descList,
                        TArgPairList& requiredList,
                        TArgPairList& optionalList) override;

        // ILogMessage
        void    onModuleMessage(eSeverity severity,
                                const std::string& msg) override;
    private:
        bool setupFromYaml(void);
        bool setupFromCommand(void);

    private:
        std::mutex          _mutex;
        ServerConfig        _serverConfig;
        ClientListener      _clientListener;
        ActiveClientMgr     _activeClientMgr;
};

#endif // AB_GAME_SERVER_H

