#pragma once

#include <QString>
#include <QVector>
#include <QList>

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
	SqlTypes type = SqlTypes::eMYSQL;
	QString host;
	QString user;
	QString passwd;
	QString dbName;
	QString port;

	QString toString()
	{
		return QString::fromStdWString(L"IP地址:%1;端口:%2;用户名:%3;数据库名:%4;")
			.arg(host).arg(port).arg(user).arg(dbName);
	}
}SqlBaseInfo, *pSqlBaseInfo;

typedef struct _SqlTableInfo_
{
	SqlBaseInfo baseInfo;
	QString tableName;

	QString toString()
	{
		return QString::fromStdWString(L"%1表名:%2;")
			.arg(baseInfo.toString()).arg(tableName);
	}
}SqlTableInfo, * pSqlTableInfo;

typedef struct _DbFieldGroup_
{
	QVector<QString> fields;
}DbFieldGroup, * pDbFieldGroup;

typedef struct _DbData_
{
	DbFieldGroup fieldGroup;
	QList<DbFieldGroup> rows;
}DbData, * pDbData;
