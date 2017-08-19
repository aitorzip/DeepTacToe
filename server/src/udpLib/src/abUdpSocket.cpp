#include "abAppTools.h"  // Must be very first file
#include "abUdpSocket.h"
#include "abAppToolsBuildType.h"

#if ENABLE_UDP_SRC_CHECK
    #define CHECK_SOURCE_IP_START()         \
        do {

    #define CHECK_SOURCE_IP_END(srcF, scrR) \
        } while( srcF && srcF != scrR );
#else
    #define CHECK_SOURCE_IP_START()
    #define CHECK_SOURCE_IP_END(srcF, scrR)
#endif


UDPSocket::UDPSocket(void) : INetSocket(ISocket::eUDP)
{
    STACK
}

UDPSocket::~UDPSocket(void)
{
    STACK
}

void UDPSocket::connect(void) noexcept(false)
{
    STACK

    onStatusEvt.publish(hostAndPort(), ISocket::eConnected);
}

void UDPSocket::shutdown(void)
{
    STACK

    onStatusEvt.publish(hostAndPort(), ISocket::eDisconnected);
}

bool UDPSocket::pull(void)
{
    STACK

    return false;
}

bool UDPSocket::spin(void)
{
    STACK

    return false;
}

int32_t UDPSocket::send( int8_t data ) const
{
    STACK

    return sendto(_fd[0], &data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( uint8_t data ) const
{
    STACK

    return sendto(_fd[0], &data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( int16_t data ) const
{
    STACK

    data = HOST_TO_NET_S( data );
    return sendto(_fd[0], (const uint8_t*)(&data), sizeof(data), 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( uint16_t data ) const
{
    STACK

    data = HOST_TO_NET_S( data );
    return sendto(_fd[0], (const uint8_t*)(&data), sizeof(data), 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( int32_t data ) const
{
    STACK

    data = HOST_TO_NET_L( data );

    return sendto(_fd[0], (const uint8_t*)(&data), sizeof(data), 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( uint32_t data ) const
{
    STACK

    data = HOST_TO_NET_L( data );

    return sendto(_fd[0], (const uint8_t*)(&data), sizeof(data), 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( const int8_t* pData, uint32_t size) const
{
    STACK

    TEST_ASSERT( size == 0, "Trying to send zero length data");
    TEST_ASSERT( pData == nullptr, "Trying to send nullptr buffer");

    return sendto(_fd[0], pData, size, 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::send( const uint8_t* pData, uint32_t size) const
{
    STACK

    TEST_ASSERT( size == 0, "Trying to send zero length data");
    TEST_ASSERT( pData == nullptr, "Trying to send nullptr buffer");

    return sendto(_fd[0], pData, size, 0, (SOCKADDR*)&_sockAddr, sizeof( _sockAddr ) );
}

int32_t UDPSocket::receive( int8_t&   data ) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], &data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    return ret;
}

int32_t UDPSocket::receive( uint8_t&  data ) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], &data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    return ret;
}

int32_t UDPSocket::receive( int16_t&  data ) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], (uint8_t*)&data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    data = NET_TO_HOST_S( data );

    return ret;
}

int32_t UDPSocket::receive( uint16_t& data ) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], (uint8_t*)&data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    data = NET_TO_HOST_S( data );

    return ret;
}

int32_t UDPSocket::receive( int32_t&  data ) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], (uint8_t*)&data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    data = NET_TO_HOST_L( data );

    return ret;
}

int32_t UDPSocket::receive( uint32_t& data ) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], (uint8_t*)&data, sizeof(data), 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    data = NET_TO_HOST_L( data );

    return ret;
}

int32_t UDPSocket::receive( int8_t* data, uint32_t size) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], data, size, 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    return ret;
}

int32_t UDPSocket::receive( uint8_t* data, uint32_t size) const
{
    STACK

    uint32_t addrlen = sizeof(_sockAddr);

    int ret;

    CHECK_SOURCE_IP_START()
        if( (ret = recvfrom(_fd[0], data, size, 0, (SOCKADDR*)&_sockAddr, &addrlen) ) <= 0)
            return ret;

    CHECK_SOURCE_IP_END(_sourceIP, _servaddr.sin_addr.s_addr )

    return ret;
}

std::ostream& operator <<(std::ostream &os, const UDPSocket& obj)
{
    STACK
    
    os << "UDP " << obj._ifName << " Port " << obj._port;

    return os; 
}
