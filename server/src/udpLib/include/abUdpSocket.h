#ifndef AB_UDP_SOCKET_H
#define AB_UDP_SOCKET_H

// UDPSocket - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abINetSocket.h"
#include <stdexcept>

class UDPSocket : public INetSocket
{
        typedef Signal<const string, const int8_t*, uint32_t> TMessageEvent;

    public:
         UDPSocket(void);
        ~UDPSocket(void) override;

         /// UDP initialization
         /// hostAndPort - host:port
        void    initialize(const string& hostAndPort) noexcept(false);

        /// UDP initialization
        /// host - ip or host name
        /// port - port
        void    initialize(const string& host, uint16_t port) noexcept(false);

        void    connect(void)    noexcept(false) override;
        void    shutdown(void)   override;
        bool    pull(void)       override;
        bool    spin(void)       override;

        int32_t send( int8_t            data ) const override;
        int32_t send( uint8_t           data ) const override;
        int32_t send( int16_t           data ) const override;
        int32_t send( uint16_t          data ) const override;
        int32_t send( int32_t           data ) const override;
        int32_t send( uint32_t          data ) const override;
        int32_t send( const int8_t*     pData, uint32_t size) const override;
        int32_t send( const uint8_t*    pData, uint32_t size) const override;

        int32_t receive( int8_t&        data ) const override;
        int32_t receive( uint8_t&       data ) const override;
        int32_t receive( int16_t&       data ) const override;
        int32_t receive( uint16_t&      data ) const override;
        int32_t receive( int32_t&       data ) const override;
        int32_t receive( uint32_t&      data ) const override;
        int32_t receive( int8_t*        data, uint32_t size) const override;
        int32_t receive( uint8_t*       data, uint32_t size) const override;


        TMessageEvent   onDataEvt;

    private:

        void    threadHandler(void);

    // Friends
        friend std::ostream& operator<<(std::ostream &os, const UDPSocket& obj);
};

std::ostream& operator <<(std::ostream &os, const UDPSocket& obj);

#endif // AB_UDP_SOCKET_H

