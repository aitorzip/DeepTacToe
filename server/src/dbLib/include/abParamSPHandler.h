#ifndef AB_PARAM_PROC_HANDLER_H
#define AB_PARAM_PROC_HANDLER_H

#include "abStoredProcHandler.h"
#include <list>

// ParamSPHandler - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

class ParamSPHandler : public StoredProcHandler
{
    public:
        typedef list< pair< string, string> >   TPairVal;

                 ParamSPHandler(void);
        virtual ~ParamSPHandler(void) {}

        inline const TPairVal&  Values(void) const { return _values; }

    protected:
        virtual void    OnRecordHandler(    const MYSQL_FIELD*  pMetadata,
                                            uint32_t            metaCount,
                                            const MYSQL_ROW&    row) throw(std::runtime_error);

    private:
        TPairVal    _values;
};

#endif // AB_PARAM_PROC_HANDLER_H

