#include <sstream>
#include <errmsg.h>
#include <sstream>
#include <cassert>
#include "abMySQLDb.h"

uint32_t MySQLDb::_refCounter = 0;

const string MySQLDb::DB_USER_NAME("autobon");
const string MySQLDb::DB_USER_PASSWORD("pazzword");

#define MYSQL_VER std::string("MySQL Ver    : ")

void    MySQLDb::InitResources(void)
{
    _lock.lock();
        if( _refCounter == 0 )
            mysql_library_init(0, 0L, 0L);

        _refCounter++;
    _lock.unlock();
}

void    MySQLDb::ReleaseResources(void)
{
    _lock.lock();

    --_refCounter;

    if( _refCounter == 0 )
        mysql_library_end();

    _lock.unlock();
}

MySQLDb::MySQLDb(void)
 : _pMySql(0L)
{
    InitResources();
}

MySQLDb::MySQLDb( const string& hostName, const string& userName,
                  const string& passwd, const string& dbName,
                  uint32_t dbPort, bool enableCompression)
 : _pMySql(0L), _hostName(hostName), _databaseName(dbName),
   _user(userName), _passWord(passwd), _dbPort(dbPort),
   _enableCompression(enableCompression)
{
    if( _hostName.length() == 0 )
        _hostName = "localhost";

    assert( _databaseName.length() );

    InitResources();
}

MySQLDb::~MySQLDb() 
{
    try { Disconnect(); } catch(...) {}

    ReleaseResources();
}

void    MySQLDb::SelectDatabase(const string& database) throw(std::runtime_error)
{
    if( _pMySql == 0L )
        throw std::runtime_error("Please connect to database first");

    if( database.length() == 0 )
        throw std::runtime_error("Missing DB name");

    if( _databaseName == database )
        return;

    _databaseName = database;

    if( mysql_select_db( _pMySql, _databaseName.c_str() ) != 0 )
    {
        string message("Database select failed: ");
               message += mysql_error( _pMySql );
        throw std::runtime_error(message);
    }
}

void    MySQLDb::ConvertToBlob(const string& dataIn, string& dataOut)
{
    char* msgCopy = new char[ (dataIn.size() * 2) + 1 ];

    mysql_escape_string( msgCopy, dataIn.c_str(), (unsigned long) dataIn.size() );

    dataOut = msgCopy;

    delete [] msgCopy;
}

void    MySQLDb::InitDB( const string& hostName, const string& userName,
                         const string& password, const string& dbName,
                         uint32_t dbPort, bool enableCompression)
{
    if ( ! mysql_thread_safe() )
         throw std::runtime_error("Thread Safe Is OFF");

    assert( dbName.length() );

    _hostName           = hostName;
    _databaseName       = dbName;
    _user               = userName;
    _passWord           = password;
    _dbPort             = dbPort;
    _enableCompression  = enableCompression;

    if( _hostName.length() == 0 )
        _hostName = "localhost";
}

void    MySQLDb::Connect(void) throw(std::runtime_error)
{
    InitDBLib();

    unsigned long clientflag = CLIENT_MULTI_STATEMENTS | CLIENT_REMEMBER_OPTIONS; // | CLIENT_FOUND_ROWS

    if( _enableCompression )
        clientflag |= CLIENT_COMPRESS;

	if( ! mysql_real_connect( _pMySql,
                              _hostName.c_str(),
                              _user.c_str(),
                              _passWord.c_str(),
				              _databaseName.c_str(),
                              _dbPort,
                              NULL,
                              clientflag))
    {
        string message("Failed on Database connect: ");
               message += mysql_error( _pMySql );

        Disconnect();
        throw std::runtime_error(message);
    }

    // Reconnect has some expectations. The connection-related state is affected as follows
    // in http://dev.mysql.com/doc/refman/5.5/en/auto-reconnect.html

    my_bool  my_true = 1;

    if( mysql_options( _pMySql, MYSQL_OPT_RECONNECT, &my_true) != 0)
    {
        mysql_close( _pMySql );
        delete _pMySql; _pMySql = 0L;
        throw std::runtime_error("Failed to enable Reconnect option on DB");
    }
}

void    MySQLDb::Disconnect() throw(std::runtime_error)
{
    if( _pMySql )
    {
        mysql_close( _pMySql ); 
        delete _pMySql;
        _pMySql = 0L;
    }

    ThreadUninitialize();
}

void    MySQLDb::ExecuteSQL(const string& sqlCommand, StoredProcHandler& spHandler) throw(std::runtime_error)
{
    if( _pMySql == 0L )
        throw std::runtime_error("Please connect to database first");

    TLockGuard scopedMtx(_lock);

    uint32_t  retry(0);
TryAgain:

    int retVal = mysql_real_query( _pMySql, sqlCommand.c_str(), (unsigned long)(sqlCommand.length()) );

    if( retVal != 0 )
    {
        uint32_t mysqlErr = mysql_errno( _pMySql );

        if( mysqlErr == 1205 ) //  Lock wait timeout exceeded; try restarting transaction
        {
            if( retry < 3 )
            {
                retry++;
                goto TryAgain;
            }
        }
        else if(mysqlErr == 2013)
        {
            if( retry < 1 )
            {
                retry++;
                mysql_ping(_pMySql);
                goto TryAgain;
            }
        }

	    std::ostringstream message;

        message << "ExecuteSQL(I) Failed: ";
        message << sqlCommand << " - SqlErr: " << mysqlErr;
        message << " SQL State: ";
        message << mysql_sqlstate( _pMySql );

        if( retry )
            message << ". Attempt: " << retry;

        throw std::runtime_error(message.str());
    }

    MYSQL_RES* pResult;
    MYSQL_ROW  record;

    while(true) 
    {
        pResult = mysql_store_result( _pMySql );

        if( pResult )
        {
            unsigned int  numFields = mysql_num_fields( pResult );
            MYSQL_FIELD*  pFields   = mysql_fetch_fields( pResult );

            if( numFields )
            {
                try
                {
                    if( spHandler._eHandlerType == StoredProcHandler::eRECORD )
                    {
                        spHandler.OnRowHandler( pFields, numFields, pResult); 
                    }
                    else    // StoredProcHandler.eROW
                    {
                        while( (record = mysql_fetch_row( pResult )) )
                            spHandler.OnRecordHandler( pFields, numFields, record ); 
                    }
                }
                catch(const std::exception& ex)
                {
                    mysql_free_result( pResult );
                    throw;
                }
            }

            mysql_free_result( pResult );
        }

        int val = mysql_next_result( _pMySql );

        if( val == 0 )
            continue;       // We have more data
        else if( val == -1)
            break;          // No more records
        else
        {
            string message("Failed Getting Next Result. SQLErr: ");
                   message += mysql_error( _pMySql );
                   message += " SQL State: ";
                   message += mysql_sqlstate( _pMySql );

            throw std::runtime_error(message);
        }
    }
}

void    MySQLDb::ExecuteSQL(const string& sqlCommand) throw(std::runtime_error)
{
    if( _pMySql == 0L )
        throw std::runtime_error( "Please Connect To Database First" );

    uint32_t  retry(0);

    TLockGuard scopedMtx(_lock);

TryAgain:

    int retVal = mysql_real_query( _pMySql, sqlCommand.c_str(), (unsigned long)(sqlCommand.length()) );

    if( retVal != 0 )
    {
        int32_t mysqlErr = mysql_errno( _pMySql );

        if( mysqlErr == 1205 ) //  Lock wait timeout exceeded; try restarting transaction
        {
            if( retry < 3 )
            {
                retry++;
                goto TryAgain;
            }
        }
        else if(mysqlErr == 2013)
        {
            if( retry < 1 )
            {
                retry++;
                mysql_ping(_pMySql);
                goto TryAgain;
            }
        }

	    ostringstream message;

        message << "ExecuteSQL(N) Failed: ";
        message << sqlCommand << " - SqlErr: " << mysqlErr;
        message << " SQL State: ";
        message << mysql_sqlstate( _pMySql );

        if( retry )
            message << ". Attempt# " << retry;

        throw std::runtime_error(message.str());
    }

    //while( mysql_next_result(_pMySql) == 0 );    // This is a must here to iterate over any returned data else 2014 error will happen
                                                   // If expecting data use other ExcecuteSQL call

    while(true) 
    {
        MYSQL_RES* pResult = mysql_store_result( _pMySql );

        if( pResult )
            mysql_free_result( pResult );

        int val = mysql_next_result( _pMySql );

        if( val == 0 )
            continue;       // We have more data
        else
            break;          // No more records
    }
}

string  MySQLDb::GetDBInfo(void) throw(std::runtime_error)
{
    if( _pMySql == 0L )
        InitDBLib(); // InitDBLib can THROW_APP_EXCEPTION

    string  message( MYSQL_VER );
            message.append( "Linked [" );
            message.append( mysql_get_client_info() );
            message.append( "] Svr [" );
            message.append( mysql_get_server_info(_pMySql) );
            message.append( "]" );

    return  message;
}

bool    MySQLDb::IsConnected(void) const
{
    if( _pMySql )
        return true;
    else return false;
}


void    MySQLDb::AutoCommit(bool on) throw(std::runtime_error)
{
    string  sqlMessage("SET SESSION autocommit=");

    if( on )
        sqlMessage.append("1");
    else
        sqlMessage.append("0");

    ExecuteSQL(sqlMessage);
}

void    MySQLDb::Transaction(eSQLTrans on) throw(std::runtime_error)
{
    if( on == eSQLTrans_Begin )
        ExecuteSQL("BEGIN");
    else if( on == eSQLTrans_Commit )
        ExecuteSQL("COMMIT");
    else if( on == eSQLTrans_RollBack )
        ExecuteSQL("ROLLBACK");
}

void    MySQLDb::InitDBLib(void) throw(std::runtime_error)
{
    if( _pMySql )
        return;

    _pMySql = new MYSQL;

    if ( mysql_init( _pMySql) == NULL )
    {
        delete _pMySql;
        _pMySql = 0L;
        throw std::runtime_error( "Failed to initialize DB" );
    }

    ThreadInitialize();
}

void    MySQLDb::ThreadInitialize(void)
{
    if( mysql_thread_init() )
        throw std::runtime_error( "Thread Initializetion Failed" );
}

void    MySQLDb::ThreadUninitialize(void)
{
    mysql_thread_end();
}

