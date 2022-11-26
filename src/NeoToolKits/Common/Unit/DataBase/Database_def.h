#pragma once

#include <windows.h>
#include <iostream>
#include <string.h>

typedef enum
{
	eMYSQL,
	//eORACLE,
	eSQLITE,
	//ePostgreSQL,
	//eMSSQL,
}SqlTypes;

typedef struct _SqlBaseInfo_
{
	SqlTypes type;
	std::string host;
	std::string user;
	std::string passwd;
	std::string db;
	unsigned int port;
}SqlBaseInfo, *pSqlBaseInfo;

typedef struct _SqlTableInfo_
{
	SqlBaseInfo baseInfo;
	std::string table;
}SqlTableInfo, *pSqlTableInfo;
