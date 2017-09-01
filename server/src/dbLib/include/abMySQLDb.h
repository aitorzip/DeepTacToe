#ifndef AB_MYSQL_DB_H
#define AB_MYSQL_DB_H

// MySQLDb - Proprietary and confidential
// Author Robert J. Gebis
// Copyright (C) Autobon.ai, LLC - All Rights Reserved
// Unauthorized copying of this file via any medium
// is strictly prohibited without the written permission

#include <mutex>
#include "abStoredProcHandler.h"

class MySQLDb
{
    public:  
        typedef lock_guard<mutex>   TLockGuard;

        enum eSQLTrans
        {
            eSQLTrans_Begin,
            eSQLTrans_Commit,
            eSQLTrans_RollBack
        };

        // If dafault ctor used you must call InitDB first before Connect
        MySQLDb(void); 
        // If using ctor and passing db info you do not have to call InitDB before Connect
        MySQLDb( const string& hostName,
                 const string& userName,
                 const string& password,
                 const string& dbName,
                 uint32_t dbPort=0, bool enableCompression=false);    // Use default MySQL port

        virtual ~MySQLDb();

        virtual void    InitDB( const string& hostName,
                                const string& userName,
                                const string& password,
                                const string& dbName,
                                uint32_t       dbPort=3306,
                                bool          enableCompression=false);     // Use default MySQL port

        void    Connect(void) throw(std::runtime_error);
        void    Disconnect(void) throw(std::runtime_error);
        void    SelectDatabase(const string& database) throw(std::runtime_error);
        bool    IsConnected(void) const;
        string  GetDBInfo(void) throw(std::runtime_error);
        void    AutoCommit(bool on) throw(std::runtime_error);
        void    Transaction(eSQLTrans on)  throw(std::runtime_error);

        void    ThreadInitialize(void);
        void    ThreadUninitialize(void);

        inline const string&        Host(void)        const { return _hostName; }
        inline const string&        DBName(void)      const { return _databaseName; }
        inline const string&        User(void)        const { return _user; }
        inline const string&        Password(void)    const { return _passWord; }
        inline       uint32_t       Port(void)        const { return _dbPort; }
        inline       bool           Compression(void) const { return _enableCompression; }

        void    ConvertToBlob(const string& dataIn, string& dataOut);

        void    ExecuteSQL(const string& sqlCommand, StoredProcHandler& spHandler) throw(std::runtime_error);
        void    ExecuteSQL(const string& sqlCommand) throw(std::runtime_error);
    protected:
        void    InitDBLib(void) throw(std::runtime_error);

    protected:
        MYSQL*         _pMySql;
        string         _hostName; 
        string         _databaseName; 
        string         _user; 
        string         _passWord; 
        uint32_t       _dbPort;
        bool           _enableCompression;

        std::mutex     _lock;

        void            InitResources(void);
        void            ReleaseResources(void);

        static uint32_t _refCounter;

    public:
        static const string DB_USER_NAME;
        static const string DB_USER_PASSWORD;
};

#endif // AB_MYSQL_DB_H


