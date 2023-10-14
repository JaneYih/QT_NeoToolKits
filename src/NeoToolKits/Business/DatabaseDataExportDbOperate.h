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
		bool QueryDataByIndexCondition(const ExportConfig& queryCfg, DataTable& outputData, QString& strErrorMsg);

	private:
		QString CreateSelectDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values);
		bool ExcuteDataSelectCommand(const QString& cmd, DataTable& outputData, QString& strErrorMsg);

	private:
		SqlTableInfo m_stSqlInfo;
	};
};
