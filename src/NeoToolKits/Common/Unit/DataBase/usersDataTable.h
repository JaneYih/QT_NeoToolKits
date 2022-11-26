#pragma once

#include "ITableManage.h"

class CUsersDataTable :public ITableManage
{
public:
	CUsersDataTable(const string& db) :ITableManage("localhost", "root", "root", db, 3306)
	{
		CreateTable();
	}

	virtual ~CUsersDataTable()
	{
	}

public:
	virtual bool CreateTable(); 
	virtual bool InsertData(vector<string> para); 
	virtual bool DeleteData(vector<string> para); 
	virtual bool UpdataData(vector<string> para); 
	virtual bool SelectData(vector<string> para, DataTable& outputData);  
};