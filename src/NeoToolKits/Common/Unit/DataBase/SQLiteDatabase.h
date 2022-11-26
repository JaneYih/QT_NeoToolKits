#ifndef SQLISTDB_H
#define SQLISTDB_H

#include <QObject>
#include "DatabaseBaseClass.h"

class CSQLiteDatabase : public QObject, public IDatabase
{
	Q_OBJECT
public:
	explicit CSQLiteDatabase(const string& db, QObject *parent = nullptr);
	virtual ~CSQLiteDatabase() { UnInit(); }

private:
	int InitErr;
	QString m_strDataBaseName;

public:
	virtual bool IsInit();
	virtual int Init();
	virtual int UnInit();
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

signals:

public slots:

};

#endif 
