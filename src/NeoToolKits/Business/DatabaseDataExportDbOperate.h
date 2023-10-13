#pragma once

#include "ITableManage.h"
#include "DatabaseDataExport_def.h"

namespace NAMESPACENAME_DATABASE_DATA_EXPORT
{
	class DatabaseDataExportDbOperate :public ITableManage
	{
	public:
		DatabaseDataExportDbOperate(const SqlTableInfo& info);
		virtual ~DatabaseDataExportDbOperate();

	public:
		bool test(QString& strErrMsg);
		//bool InsertExcelData(const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);
		//bool UpdateExcelData_EmptyFill(const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);
		//bool UpdateExcelData_Rewrite(const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);

	private:
		//QString GenerateInsertCommand(const QString& tableName,
		//	const QVector<UploadData>& data,
		//	const QString& strProductionOrderID,
		//	QString& displayText);
		//QString GenerateUpdateCommand(const QString& tableName,
		//	const QVector<UploadData>& data,
		//	const QString& strProductionOrderID,
		//	QString& displayText);
		//QString GenerateClearDataCommand(const QString& tableName,
		//	const QVector<UploadData>& data,
		//	const QString& strProductionOrderID,
		//	QString& displayText);

		//bool OperateExcelData(UploadOptions type, const QList<QVector<UploadData>>& dataList, const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo);

	private:
		SqlTableInfo m_stSqlInfo;
	};
};
