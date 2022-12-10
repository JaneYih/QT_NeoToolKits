#pragma  once

#include <windows.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <list>
#include "Database_def.h"
using namespace std;

typedef struct 
{
	vector<string> FieldListValue;
}FieldList;

typedef struct 
{
	FieldList FieldName;      
	vector<FieldList> RowList; 
}DataTable;

class IDatabase
{
public:
	IDatabase(){}
	virtual ~IDatabase(){}

public:
	virtual bool IsInit() = 0;    //初始化完成
	virtual int Init() = 0;    //初始化
	virtual int UnInit() = 0;  //反初始化

	virtual int ExcuteCommand(const char*  command) = 0;
	virtual int GetResultData(const char*  command, DataTable& ResultData) = 0;

	virtual int BeginTransaction() = 0;
	virtual int RollBackTransaction(const char* PointName) = 0;
	virtual int SetRollBackPoint(const char* PointName) = 0;
	virtual int CommitTransaction() = 0;

	virtual int CreateIndex(const char* TableName, const char* FieldName, const char* IndexName) = 0;
	virtual int DeleteIndex(const char* TableName, const char* FieldName, const char* IndexName) = 0;
	virtual int ShowIndex(const char* TableName) = 0;

	virtual bool IsExistTable(const char* TableName) = 0;
	virtual bool GetTableFullFields(const char* TableName, std::list<string>& Fields) = 0;
	virtual string GetLastError() = 0;
};
