#ifndef AB_SERVER_CONFIG_H
#define AB_SERVER_CONFIG_H

// ServerConfig - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <list>
#include <string>

class ServerConfig
{
    public:
        ServerConfig(void);

        void    loadFromFile(  const std::string& file);
        void    loadFromBuffer(const std::string& yaml);

        inline bool         loaded(void)            const { return _loaded; }
        inline std::string  iFaceAndPort(void)      const { return _iFaceAndPort; }
        inline uint32_t     clientsPerThread(void)  const { return _clientPerThread; }

        static const std::string genTemplate(void);

    private:
        bool        _loaded;
        std::string _iFaceAndPort;
        uint32_t    _clientPerThread;
};

#endif // AB_SERVER_CONFIG_H
