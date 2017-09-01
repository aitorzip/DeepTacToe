#ifndef AB_STORED_PROC_HANDLER_H
#define AB_STORED_PROC_HANDLER_H

// StoredProcHandler - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <string>
#include <stdexcept>
//#include <my_global.h> // http://bugs.mysql.com/bug.php?id=62636
// MariaDB 5.5.28 has bug. Inside mysql_com.h replace with #include <winsock2.h> https://mariadb.atlassian.net/browse/MDEV-3868
#include <mysql.h>

using namespace std; 

// Base
class StoredProcHandler
{
    friend class MySQLDb;

    public:
        enum eHandlerType
        {
            eRECORD = 0,
            eROW
        };

                 StoredProcHandler( eHandlerType type ) : _eHandlerType(type) {}
        virtual ~StoredProcHandler() {}

    protected:
        // This method will be called on every row of data
        virtual void    OnRowHandler( const MYSQL_FIELD*    pMetadata,
                                      uint32_t              metaCount,
                                      const MYSQL_RES*      pRecord) throw(std::runtime_error);

        virtual void    OnRecordHandler( const MYSQL_FIELD* pMetadata,
                                         uint32_t           metaCount,
                                         const MYSQL_ROW&   row) throw(std::runtime_error);
    private:
        eHandlerType    _eHandlerType;
};

#endif // AB_STORED_PROC_HANDLER_H


