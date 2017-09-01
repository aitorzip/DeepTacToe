#include "abStoredProcHandler.h"
#include <cassert>

void    StoredProcHandler::OnRowHandler( const MYSQL_FIELD* pMetadata,
                                         uint32_t           metaCount,
                                         const MYSQL_RES*   pRecord) throw(std::runtime_error)
{
    // You should never get here
    (void)pMetadata;
    (void)metaCount;
    (void)pRecord;
    assert(0);
}

void    StoredProcHandler::OnRecordHandler( const MYSQL_FIELD*  pMetadata,
                                            uint32_t            metaCount,
                                            const MYSQL_ROW&    row ) throw(std::runtime_error)
{
    // You should never get here
    (void)pMetadata;
    (void)metaCount;
    (void)row;
    assert(0);
}
