#ifndef AB_CSV_WRITER_H
#define AB_CSV_WRITER_H

// CVSWriter - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abMacros.h"
#include "abPathUtil.h"
#include <fstream>
#include <memory>

class CVSWriter
{
    public:
        typedef std::unique_ptr<CVSWriter> TCsv;

                  CVSWriter(void)
                      : _records(0) {}
                  CVSWriter(const std::string& path)
                      : _path(path), _records(0) {}
        virtual  ~CVSWriter(void)
                  {
                      if(_stream.is_open())
                          _stream.close();
                  }

        void     open(const std::string& path)
        {
            STACK

#if DEBUG
            if(isOpen())
                throw std::runtime_error("Trying to open file multiple times" +
                                         ERROR_LOCATION);
#endif
            _path = path;

            open();
        }

        void     open(void)
        {
            STACK

#if DEBUG
            if(_path.empty())
                throw std::runtime_error("Trying open with empty path!!!" +
                                         ERROR_LOCATION);

            if(isOpen())
                throw std::runtime_error("Trying to open opened file already. Close it first!!!" +
                                         ERROR_LOCATION);
#endif
            _records = 0;
            _path    = PathUtil::forceFileExt(_path, "csv");

            _stream.open(path(), std::fstream::trunc);
        }

        bool     isOpen(void) const { return _stream.is_open(); }

        void     close(void)
        {
            STACK

            if( isOpen() )
            {
               _stream.flush();
               _stream.close();
            }
        }

        std::string path(void) const { return _path;  }

        void     write(const string& line)
        {
            STACK

            _stream << line;

            ++_records;
        }

        uint64_t records(void) const { return _records; }

    private:
        std::ofstream _stream;
        std::string   _path;
        uint64_t      _records;
};

#endif // AB_CSV_WRITER_H
