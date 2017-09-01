#include "abDBSPServiceName.h"

DBSPServiceName::DBSPServiceName(TSvrItemList& lst)
: StoredProcHandler(StoredProcHandler::eROW), _lst(lst)
{
}

void    DBSPServiceName::OnRecordHandler(   const MYSQL_FIELD*  pMetadata,
                                            uint32_t            metaCount,
                                            const MYSQL_ROW&    row) throw(std::runtime_error)
{
    SvrItem item;

    for( uint32_t Idx(0); Idx < metaCount; ++Idx)
    {
        if( string("name") == pMetadata[Idx].name )
        {
            if( row[Idx] != 0L )
               item.name = row[Idx];
        }
        else if( string("info") == pMetadata[Idx].name )
        {
            if( row[Idx] != 0L )
               item.info = row[Idx];
        }
    }

    if(  item.name.length() )
        _lst.push_back( item );
}

