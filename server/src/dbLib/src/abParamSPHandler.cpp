#include "abParamSPHandler.h"

ParamSPHandler::ParamSPHandler(void)
 : StoredProcHandler(StoredProcHandler::eROW)
{
}

void    ParamSPHandler::OnRecordHandler(    const MYSQL_FIELD*  pMetadata,
                                            uint32_t            metaCount,
                                            const MYSQL_ROW&    row) throw(std::runtime_error)
{
    _values.clear();

    for( unsigned int Idx(0); Idx < metaCount; ++Idx)
    {
        if( row[Idx] != 0L )
            _values.push_back( make_pair( pMetadata[Idx].name, row[Idx]));
        else
            _values.push_back( make_pair( pMetadata[Idx].name, ""));
    }
}
