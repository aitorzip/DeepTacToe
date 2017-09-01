#include <iostream>
#include "abStringUtil.h"
#include "abDBService.h"
#include "abDBSPServiceName.h"

void    DBService::GetServiceName(const string& spName, TSvrItemList& lst) throw(std::runtime_error)
{
    DBSPServiceName dbSPServiceName(lst);

    string sqlString("CALL ");
    sqlString.append(spName);
    sqlString.append("()");

    ExecuteSQL( sqlString, dbSPServiceName );
}

void    DBService::Print(const TSvrItemList& lst)
{
    bool infoColValid = CheckInfoColumValid(lst);

    if( infoColValid )
    {
        cout << "   ServiceInfo           Info" << endl;
        cout << "------------------------------------" << endl;
    }
    else
    {
        cout << "   ServiceInfo" << endl;
        cout << "-----------------" << endl;
    }

    if( lst.size() )
    {
        TSvrItemList::const_iterator iter = lst.begin();

        while( iter != lst.end() )
        {
            string  nameStr("   ");
                    nameStr += iter->name;

            if( infoColValid )
            {
                nameStr = StringUtil::padRight( nameStr, 25, ' ');

                cout << nameStr << ( iter->info.empty() ? "" : iter->info ) << endl;
            }
            else
                cout << nameStr << endl;

            ++iter;
        }
    }
}

bool    DBService::CheckInfoColumValid(const TSvrItemList& lst)
{
    TSvrItemList::const_iterator iter = lst.begin();

    while( iter != lst.end() )
    {
        if( iter->info.length() )
            return true;

        ++iter;
    }

    return false;
}

