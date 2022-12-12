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
	bool IsExistTable(const char* TableName);
	bool GetTableFullFields(const char* TableName, std::list<string>& Fields);
	virtual bool CreateTable();
	virtual bool InsertData(vector<string> para);
	virtual bool DeleteData(vector<string> para);
	virtual bool UpdataData(vector<string> para);
	virtual bool SelectData(vector<string> para, DataTable& outputData);

protected:
	bool AnalysisPara(list<string> para, const int paraNum, string Sqlpara[]);
};
