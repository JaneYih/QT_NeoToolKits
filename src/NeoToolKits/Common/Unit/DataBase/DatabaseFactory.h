#pragma once

#include "DatabaseBaseClass.h"
#include "MysqlDatabase.h"
#include "SQLiteDatabase.h"

typedef enum 
{
	eMYSQL,
	//eORACLE,
	eSQLITE,
	//ePostgreSQL,
	//eMSSQL,
}SqlTypes;

class CDatabaseFactory
{
public:
	CDatabaseFactory(){}
	~CDatabaseFactory(){}

public:
	static IDatabase* CreatDatabase(
		SqlTypes sql, 
		const string& host,
		const string& user,
		const string& passwd,
		const string& db,
		const unsigned int port)
	{
		switch (sql)
		{
		case eMYSQL:
			return new CMysqlDatabase(host,user,passwd,db,port);
		case eSQLITE:
			return new CSQLiteDatabase(db);
		default:
			return nullptr;
		}
	}
};