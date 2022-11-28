#pragma once
#include "DatabaseBaseClass.h"
#include "DatabaseFactory.h"
#include <stdio.h>
#include <vector>
using namespace std;

class ITableManage
{
public:
	ITableManage(const SqlBaseInfo& info)
	{
		DatabaseInstence = CDatabaseFactory::CreatDatabase(info);
	}

	virtual ~ITableManage()
	{
		delete DatabaseInstence;
	}

public:
	IDatabase* DatabaseInstence;

public:
	bool TestConnect(string& errMsg);
	virtual bool CreateTable() = 0;
	virtual bool InsertData(vector<string> para) = 0; 
	virtual bool DeleteData(vector<string> para) = 0;
	virtual bool UpdataData(vector<string> para) = 0;
	virtual bool SelectData(vector<string> para, DataTable& outputData) = 0;

protected:
	bool IsExistTable(const char* TableName);
	bool AnalysisPara(list<string> para, const int paraNum, string Sqlpara[]);
};
