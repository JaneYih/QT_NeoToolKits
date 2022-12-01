#pragma once

#include <QObject>
#include "IniOperation.h"
#include "Database_def.h"

class ExcelDataUploadApp  : public QObject
{
	Q_OBJECT

public:
	ExcelDataUploadApp(QObject *parent);
	~ExcelDataUploadApp();

	QStringList LoadExcelColumns(const QString& fileName);

	QString getExcelFileName() const;
	QString getIniFileName() const;
	QString getIniSqlCfgPrefix() const;
	SqlTableInfo* getSqlTableInfoPointer() const;

private:
	void setExcelFileName(const QString& filename);

private:
	SqlTableInfo m_stTableInfo;
	QString m_strIniFileName;
	IniOperation* m_pCfg;
	QString m_strExcelFileName;
};
