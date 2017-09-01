#ifndef AB_SP_GET_SERVICE_LIST_H
#define AB_SP_GET_SERVICE_LIST_H

// DBSPServiceName - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abStoredProcHandler.h"
#include "abDBService.h"

class DBSPServiceName : public StoredProcHandler
{
    public:
                  DBSPServiceName(TSvrItemList& lst);
         virtual ~DBSPServiceName(void){}

    protected:
        virtual void    OnRecordHandler(    const MYSQL_FIELD*  pMetadata,
                                            uint32_t             metaCount,
                                            const MYSQL_ROW&    row) throw(std::runtime_error);

    private:
        TSvrItemList& _lst;
};

#endif // AB_SP_GET_SERVICE_LIST_H

