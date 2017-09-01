#ifndef AB_ERROR_PROC_HANDLER_H
#define AB_ERROR_PROC_HANDLER_H

// ErrorSPHandler - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include "abStoredProcHandler.h"

class  ErrorSPHandler : public StoredProcHandler
{
   public:
        ErrorSPHandler(void);

        inline const   string&  Error(void)     const { return _error; }

    protected:
        virtual void  OnRecordHandler(const MYSQL_FIELD* pMetadata,
                                      uint32_t           metaCount,
                                      const MYSQL_ROW&   row) throw(std::runtime_error);

    protected:
        string  _error;
};

#endif // AB_ERROR_PROC_HANDLER_H

