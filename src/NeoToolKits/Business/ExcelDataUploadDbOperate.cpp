#include "ExcelDataUploadDbOperate.h"


ExcelDataUploadDbOperate::ExcelDataUploadDbOperate(const SqlTableInfo& info)
	: ITableManage(info.baseInfo),
	m_stSqlInfo(info)
{
	
}

ExcelDataUploadDbOperate::~ExcelDataUploadDbOperate()
{
}

//bool ExcelDataUploadDbOperate::InsertData(vector<string> para)
//{
//	if (!DatabaseInstence->IsInit())
//	{
//		return false;
//	}
//
//	int res = 0;
//	char command[5120] = { 0 };
//	if (para.size() >= 3)
//	{
//		sprintf_s(command, _insertSql, para[0].c_str(), para[1].c_str(), para[2].c_str());
//	}
//	else
//	{
//		return false;
//	}
//
//	res |= DatabaseInstence->BeginTransaction();
//
//	res |= DatabaseInstence->ExcuteCommand(command);
//	//res |= DatabaseInstence->RollBackTransaction(NULL);
//	res |= DatabaseInstence->CommitTransaction();
//
//	return !res;
//}

bool ExcelDataUploadDbOperate::test(QString& strErrMsg)
{
	if (!DatabaseInstence->IsInit())
	{
		strErrMsg = QString::fromStdWString(L"数据库连接失败：%1").arg(m_stSqlInfo.baseInfo.toString());
		return false;
	}

	if (!ITableManage::IsExistTable(m_stSqlInfo.tableName.toLocal8Bit()))
	{
		strErrMsg = QString::fromStdWString(L"数据表<%1>不存在").arg(m_stSqlInfo.tableName);
		return false;
	}

	return true;
}

