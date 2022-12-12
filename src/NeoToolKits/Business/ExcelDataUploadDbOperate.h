#pragma once

#include "ITableManage.h"

class ExcelDataUploadDbOperate :public ITableManage
{
public:
	ExcelDataUploadDbOperate(const SqlTableInfo& info);
	virtual ~ExcelDataUploadDbOperate();

public:
	bool test(QString& strErrMsg);

private:
	SqlTableInfo m_stSqlInfo;
};