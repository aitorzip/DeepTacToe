#ifndef AB_DB_SERVICE_H
#define AB_DB_SERVICE_H

// DBService - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abMySQLDb.h"
#include <list>

struct SvrItem
{
    string  name;
    string  info;
};

typedef list<SvrItem>   TSvrItemList;

class DBService : public MySQLDb
{
    public:
                 DBService(void){}
        virtual ~DBService(void){}

        void     GetServiceName(const string& spName, TSvrItemList& lst) throw(std::runtime_error);
        void     Print(const TSvrItemList& lst);

    private:
        bool    CheckInfoColumValid(const TSvrItemList& lst);
};

#endif // AB_DB_SERVICE_H

