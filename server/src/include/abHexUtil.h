#ifndef AB_HEX_UTIL_H
#define AB_HEX_UTIL_H

// HexUtil - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abTypeDef.h"
#include "abCanBuildVersion.h"
#include "abMacros.h"

class HexUtil
{
    public:

    static string toString(const int8_t* buffer, uint32_t size, char sep = ' ')
    {
        if(buffer == nullptr or size == 0)
            return "";

        std::stringstream hex;

        for(uint32_t i(0); i < size; ++i)
        {
            if(i)
                hex << sep;

            hex << std::hex << (int32_t)buffer[i];
        }

        return hex.str();
    }
};

#endif // AB_HEX_UTIL_H
