#include "abAppTools.h"  // Must be very first file
#include "abSslSocket.h"
#include "abAppToolsBuildType.h"

SSLSocket::SSLSocket(void)
{
    STACK
}

SSLSocket::~SSLSocket(void)
{
    STACK
}

std::ostream& operator <<(std::ostream &os, const SSLSocket& obj)
{
    STACK

    os << "SSL " << obj._ifName << " Port " << obj._port;

    return os;
}

