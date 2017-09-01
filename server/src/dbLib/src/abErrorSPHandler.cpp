#include "abErrorSPHandler.h"

ErrorSPHandler::ErrorSPHandler(void)
: StoredProcHandler(StoredProcHandler::eROW)
{}

void  ErrorSPHandler::OnRecordHandler( const MYSQL_FIELD* pMetadata,
                                       uint32_t           metaCount,
                                       const MYSQL_ROW&   row) throw(std::runtime_error)
{
    for( unsigned int Idx(0); Idx < metaCount; ++Idx)
    {
        if( string("error") == pMetadata[Idx].name )
            _error = row[Idx];
    }
}
