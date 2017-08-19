#ifndef AB_SSL_SOCKET_H
#define AB_SSL_SOCKET_H

// SSLSocket - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abTcpSocket.h"

class SSLSocket : public TCPSocket
{
    public:
         SSLSocket(void);
        ~SSLSocket(void) override;

    private:

    // Friends
        friend std::ostream& operator<<(std::ostream &os, const SSLSocket& obj);
};

std::ostream& operator <<(std::ostream &os, const SSLSocket& obj);

#endif // AB_SSL_SOCKET_H

