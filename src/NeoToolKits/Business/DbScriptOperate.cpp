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
	QString strErrorMsg;
	return GetTableFullFields(Fields, strErrorMsg);
}

bool DbScriptOperate::GetTableFullFields(DbFieldGroup& outFields, QString& strErrorMsg)
{
	outFields.clear();
	if (!DatabaseInstence->IsInit())
	{
		strErrorMsg = QString::fromStdWString(L"���ݿ�δ����");
		return false;
	}

	QByteArray byteTableName = m_stSqlInfo.tableName.toLocal8Bit();
	if (DatabaseInstence->IsExistTable(byteTableName))
	{
		FieldList Fields;
		DatabaseInstence->GetTableFullFields(byteTableName, Fields);
		outFields = Fields;
		return outFields.fields.size() > 0;
	}
	strErrorMsg = QString::fromStdWString(L"���ݿ��(%1)������").arg(m_stSqlInfo.tableName);
	return false;
}

bool DbScriptOperate::GetTableFullData(DbData& outData, QString& strErrorMsg)
{
	outData.clear();
	if (!DatabaseInstence->IsInit())
	{
		strErrorMsg = QString::fromStdWString(L"���ݿ�δ����");
		return false;
	}

	DataTable ResultData;
	if (0 == DatabaseInstence->GetResultData(QString("SELECT * FROM %1;").arg(m_stSqlInfo.tableName).toLocal8Bit(), ResultData))
	{
		outData = ResultData;
		return true;
	}
	strErrorMsg = QString::fromStdString(DatabaseInstence->GetLastError());
	return false;
}