#ifndef AB_READ_BUFFER_H
#define AB_READ_BUFFER_H

// ReadBuffer - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

class ReadBuffer
{
    public:
        ReadBuffer(void)
            : _size(16), _shrinkCount(0), _buffer(new int8_t[16]) {}

        ~ReadBuffer(void)
        {
            if(_buffer)
                delete [] _buffer;
        }

        void realloc(uint32_t s)
        {
            // Every 50 resizes make srue buffer is never hudge
            if(s > _size or _shrinkCount == 50)
            {
                delete [] _buffer;
                _buffer      = new int8_t[s+1];
                _size        = s;
                _shrinkCount = 0;
            }
            else
                _shrinkCount++;
        }

        int8_t*   buffer(void) { return _buffer; }

    private:
        uint32_t  _size;
        uint32_t  _shrinkCount;
        int8_t*   _buffer;
};

#endif // AB_READ_BUFFER_H
