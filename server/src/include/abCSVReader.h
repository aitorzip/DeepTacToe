#ifndef AB_CSV_READER_H
#define AB_CSV_READER_H

// CVSReader - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abSigSlot.h"
#include "abStringUtil.h"
#include "abPathUtil.h"

class CVSReader
{
    public:
        typedef Signal<const TStringVec& > TDataEvent;

        TDataEvent    onDataEvt;

        CVSReader(void) : _records(0) {}

        uint32_t  records(void) const { return _records; }

        void    read(const std::string& csvPath, const char sep = ',')
        {
#if DEBUG
            if(onDataEvt.count() == 0)
                throw std::runtime_error("No connected Slot!!" +
                                         ERROR_LOCATION);
#endif

            if(_stream.is_open())
                _stream.close();

            _records = 0;

            std::string                 line;
            std::vector<std::string>    vec;

            _stream.open(csvPath);

            if (_stream.is_open() and getline (_stream, line) )
            {
                StringUtil::split(vec, line, sep);

                if(vec.size() != 11 or vec[0] != "TIME" or vec[2] != "PGN")
                    throw std::runtime_error("Invalid file format. " +
                                             ERROR_LOCATION);

                while ( getline (_stream, line) )
                {
                    _records++;

                    StringUtil::split(vec, line, sep);

                    try {
                        onDataEvt.publish(vec);
                    }
                    catch(const std::runtime_error& ex)
                    {
                        throw std::runtime_error(string(ex.what()) +
                                                 " in " + csvPath +
                                                 " at " + std::to_string(_records) +
                                                 ERROR_LOCATION);
                    }
                }

                _stream.close();
            }
            else
                throw std::runtime_error("Can't open: " + csvPath +
                                         ", Reason: " + std::strerror(errno) + ERROR_LOCATION);
        }

    private:
        std::ifstream   _stream;
        uint32_t        _records;
};


#endif // AB_CSV_READER_H
