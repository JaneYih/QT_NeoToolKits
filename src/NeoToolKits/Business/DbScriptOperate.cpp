#include "DbScriptOperate.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DbScriptOperate::DbScriptOperate(const SqlTableInfo& info)
	: ITableManage(info.baseInfo),
	m_stSqlInfo(info)
{
	m_stSqlInfo.tableName = "model_testlist";
}

DbScriptOperate::~DbScriptOperate()
{

}

bool DbScriptOperate::TestConnect()
{
	DbFieldGroup Fields;
	return GetTableFullFields(Fields);
}

bool DbScriptOperate::GetTableFullFields(DbFieldGroup& outFields)
{
	outFields.fields.clear();
	if (!DatabaseInstence->IsInit())
	{
		return false;
	}

	QByteArray byteTableName = m_stSqlInfo.tableName.toLocal8Bit();
	if (DatabaseInstence->IsExistTable(byteTableName))
	{
		std::list<std::string> Fields;
		DatabaseInstence->GetTableFullFields(byteTableName, Fields);
		for each (auto var in Fields)
		{
			outFields.fields.push_back(QString::fromStdString(var));
		}
		return Fields.size() > 0;
	}
	return false;
}
