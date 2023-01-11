#pragma once

#include "ITableManage.h"
#include "DbScriptEditor_def.h"
#include "Database_def.h"

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DbScriptOperate;
	typedef QString (DbScriptOperate::*CreateCommandFunc)(const DbFieldGroup&, const DbFieldGroup&);
	class DbScriptOperate : public ITableManage
	{
	public:
		DbScriptOperate(const SqlTableInfo& info);
		virtual ~DbScriptOperate();

	public:
		bool TestConnect();
		bool GetTableFullFields(DbFieldGroup& Fields, QString& strErrorMsg);
		bool GetTableFullData(DbData& outData, QString& strErrorMsg, const QString& orderByField = "", Qt::SortOrder order = Qt::AscendingOrder);
		bool InsertData(const DbData& data, QString& strErrorMsg);
		bool DeleteData(const DbData& data, QString& strErrorMsg);
		bool UpdateData(const DbData& data, QString& strErrorMsg);

	private:
		bool CheckConnect(QString& strErrorMsg);
		QString CreateInsertDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values);
		QString CreateDeleteDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values);
		QString CreateUpdateDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values);
		bool ExcuteDataOperateCommand(const CreateCommandFunc createCommand, const DbData& data, QString& strErrorMsg);

	private:
		SqlTableInfo m_stSqlInfo;
	};
}


