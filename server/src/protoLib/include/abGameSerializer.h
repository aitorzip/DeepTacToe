#ifndef __AB_SERIALIZER_H__
#define __AB_SERIALIZER_H__

// Serializer - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abTcpSocket.h"
#include "abGameServerPB.pb.h"
#include "abReadBuffer.h"
#include <mutex>

using namespace ::google::protobuf;

namespace GameServerPB
{
    class Serializer
    {
        typedef std::unique_ptr<ReadBuffer> TRawBuffer;

        public:
            static const uint32_t PROTO_HEADER_SIZE = 6;

            // Returns number of bytes sent, else socket error
            // For use in single threaded mode
            static uint32_t send(const TCPSocket&   socket,
                                 eGameMsgType       type,     // defined in proto file
                                 const MessageLite& message);

            // Returns number of bytes sent, else socket error
            // For use in multi threaded mode
            static uint32_t send(std::mutex&        mutex,
                                 const TCPSocket&   socket,
                                 eGameMsgType       type,     // defined in proto file
                                 const MessageLite& message);

            static eGameMsgType msgType(const int8_t* rawBuffer);
    };
}

#endif // __AB_SERIALIZER_H__
