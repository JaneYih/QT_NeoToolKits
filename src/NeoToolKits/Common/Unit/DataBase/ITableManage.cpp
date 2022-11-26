#include "ITableManage.h"

bool ITableManage::IsExistTable(const char* TableName)
{
	if (DatabaseInstence->IsExistTable(TableName))
	{
		return true;    //数据表已存在
	}
	return false;
}

bool ITableManage::AnalysisPara(list<string> para, const int paraNum, string Sqlpara[])
{
	if (para.size() != paraNum) 
	{
		return false;
	}

	list<string>::iterator iter = para.begin();
	for (int index = 0; index < paraNum; index++)
	{
		Sqlpara[index] = *(iter++);
	}

	return true;
}

bool ITableManage::TestConnect()
{
	if (DatabaseInstence)
	{
		return DatabaseInstence->IsInit();
	}
	return false;
}
