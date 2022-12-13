#pragma once

#include "ITableManage.h"
#include "ExcelDataUpload_def.h"

using namespace ExcelDataUpload;

class ExcelDataUploadDbOperate :public ITableManage
{
public:
	ExcelDataUploadDbOperate(const SqlTableInfo& info);
	virtual ~ExcelDataUploadDbOperate();

public:
	bool test(QString& strErrMsg);
	bool InsertExcelData(const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);
	bool UpdateExcelData(const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);
	
private:
	QString GenerateInsertCommand(const QString& tableName, 
		const QVector<UploadData>& data, 
		QString& displayText);
	QString GenerateUpdateCommand(const QString& tableName,
		const QVector<UploadData>& data,
		const QString& strProductionOrderID,
		QString& displayText);
	
	enum cmdType
	{
		InsertCommand,
		UpdateCommand
	};
	bool OperateExcelData(cmdType type, const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);

private:
	SqlTableInfo m_stSqlInfo;
};