#pragma once

#include <QObject>
#include "MysqlInfoPopDialog.h"
#include "IniOperation.h"

class ExcelDataUploadApp  : public QObject
{
	Q_OBJECT

public:
	ExcelDataUploadApp(QObject *parent);
	~ExcelDataUploadApp();

	QStringList LoadExcelColumns(const QString& fileName);

	QString getExcelFileName() const;
	MysqlInfoPopDialog* getMysqlInfoDlg() const;

private:
	void setExcelFileName(const QString& filename);

private:
	QString m_strIniFileName;
	IniOperation* m_pCfg;
	QString m_strExcelFileName;
	MysqlInfoPopDialog* m_pMysqlInfoDlg;
};
