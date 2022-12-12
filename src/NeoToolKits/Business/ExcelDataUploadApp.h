#pragma once

#include <QObject>
#include "IniOperation.h"
#include "Database_def.h"
#include "ExcelDataUpload_def.h"
#include "xlsxdocument.h"
#include "xlsxcellrange.h"

class ExcelDataUploadApp  : public QObject
{
	Q_OBJECT

public:
	ExcelDataUploadApp(QObject *parent);
	~ExcelDataUploadApp();

	bool StartUpload();
	bool StopUpload();

	QStringList LoadExcelColumns(const QString& fileName);
	int LoadExcelRowCount() {
		return LoadExcelRowCount(m_strExcelFileName);
	}

	QString getExcelFileName() const;
	void setExcelFileName(const QString& filename);
	QString getIniFileName() const;
	QString getIniSqlCfgPrefix() const;
	SqlTableInfo* getSqlTableInfoPointer() const;
	void setDataMap(const QVector<ExcelDataUploadInfo>& dataMap);
	const QVector<ExcelDataUploadInfo> getDataMap() const;
	void setUploadConfig(const ExcelDataUploadConfig& stUploadConfig);
	const ExcelDataUploadConfig getUploadConfig() const;
	bool getIsUploading()const {
		return m_bUploading;
	};

private:
	QXlsx::CellRange* OpenExcelSheet(const QXlsx::Document& xlsx, int index = 0);
	int LoadExcelRowCount(const QString& fileName);

private:
	SqlTableInfo m_stTableInfo;
	QString m_strIniFileName;
	IniOperation* m_pCfg;
	QString m_strExcelFileName;
	QVector<ExcelDataUploadInfo> m_vecDataMap;
	ExcelDataUploadConfig m_stUploadConfig;
	volatile bool m_bUploading;
};
