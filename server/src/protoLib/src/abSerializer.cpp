#include "abAppToolsBuildType.h"
#include "abSerializer.h"
#include "abPathUtil.h"

namespace DeepTacToePB
{
    typedef std::lock_guard<std::mutex> TLockGuard;

    uint32_t Serializer::send(const TCPSocket&   socket,
                              eGameMsgType       type,
                              const MessageLite& message)
    {
        static thread_local TRawBuffer rawBuffer(new RawBuffer());

        int32_t byteSize  = message.ByteSize();

        rawBuffer->realloc( static_cast<uint32_t>(byteSize) + PROTO_HEADER_SIZE);

        if( message.SerializePartialToArray(rawBuffer->_buffer + PROTO_HEADER_SIZE, byteSize) == false )
            throw runtime_error("Serializer failed (" + std::to_string(type) +
                                "), Reason: SerializePartialToArray returned error" +
                                ERROR_LOCATION );

        // raw message is first 4 bytes
        *reinterpret_cast<uint32_t*>(rawBuffer->_buffer) = HOST_TO_NET_L(static_cast<uint32_t>(byteSize) + sizeof(uint16_t));

        // 5th byte set to msg type
        *reinterpret_cast<uint16_t*>(rawBuffer->_buffer + sizeof(uint32_t)) = HOST_TO_NET_S(static_cast<uint16_t>(type));

        return socket.send(rawBuffer->_buffer, static_cast<uint32_t>(byteSize) + PROTO_HEADER_SIZE);
    }

    uint32_t Serializer::send(std::mutex& mutex, const TCPSocket&   socket,
                              eGameMsgType type,  const MessageLite& message)
    {
        TLockGuard scopedMtx(mutex);

        return send(socket, type, message);
    }

    eGameMsgType Serializer::msgType(const int8_t* rawBuffer)
    {
        TEST_ASSERT(rawBuffer == nullptr, "Checking message type on invalid buffer")

        uint16_t type = NET_TO_HOST_S(*reinterpret_cast<const uint16_t*>(rawBuffer));

        if(  type > eGameFinished)
            return eUnknown;
        else
            return static_cast<eGameMsgType>(type);
    }
}
