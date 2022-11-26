#pragma once

#include "DatabaseBaseClass.h"
#include "MySQLPackage/include/mysql.h"

using namespace std;

class CMysqlDatabase : public IDatabase
{
public:
	explicit CMysqlDatabase(const SqlBaseInfo& info);
	virtual ~CMysqlDatabase();

private:
	MYSQL m_mySql;
	int m_InitErr;

	string m_strServiceIp;
	string m_strDataBaseName;
	string m_strLoginName;
	string m_strLoginPassword;
	unsigned int m_iLoginPort;

	CRITICAL_SECTION m_Critical;             
	CRITICAL_SECTION m_Transaction_Critical; 

public:
	virtual bool IsInit(); //初始化完成
	virtual int Init();    //初始化
	virtual int UnInit();  //反初始化

	virtual int ExcuteCommand(const char*  command);
	virtual int GetResultData(const char*  command, DataTable& ResultData);

	virtual int BeginTransaction();
	virtual int RollBackTransaction(const char* PointName);
	virtual int SetRollBackPoint(const char* PointName);
	virtual int CommitTransaction();

	virtual int CreateIndex(const char* TableName, const char* FieldName, const char* IndexName);
	virtual int DeleteIndex(const char* TableName, const char* FieldName, const char* IndexName);
	virtual int ShowIndex(const char* TableName);

	virtual bool IsExistTable(const char* TableName);
};

