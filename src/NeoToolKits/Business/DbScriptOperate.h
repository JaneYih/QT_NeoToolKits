#pragma once

#include "ITableManage.h"
#include "DbScriptEditor_def.h"
#include "Database_def.h"

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DbScriptOperate : public ITableManage
	{
	public:
		DbScriptOperate(const SqlTableInfo& info);
		virtual ~DbScriptOperate();

	public:
		bool TestConnect();
		bool GetTableFullFields(DbFieldGroup& Fields);

	private:
		SqlTableInfo m_stSqlInfo;
	};
}


