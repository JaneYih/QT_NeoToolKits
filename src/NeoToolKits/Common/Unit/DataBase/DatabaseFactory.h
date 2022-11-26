#pragma once

#include "DatabaseBaseClass.h"
#include "MysqlDatabase.h"
#include "SQLiteDatabase.h"

class CDatabaseFactory
{
public:
	CDatabaseFactory(){}
	~CDatabaseFactory(){}

public:
	static IDatabase* CreatDatabase(const SqlBaseInfo& info)
	{
		switch (info.type)
		{
		case eMYSQL:
			return new CMysqlDatabase(info);
		case eSQLITE:
			return new CSQLiteDatabase(info);
		default:
			return nullptr;
		}
	}
};