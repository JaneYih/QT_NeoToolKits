#pragma once
#include <QString>

#define NAMESPACENAME_DB_SCRIPT_EDITOR  Namespace_DbScriptEditor 

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	typedef struct _TestItemExcelInfo_
	{
		QString strExcelPath; //Excel路径
		QStringList listExcelColumns; //excel的标题列表

		int nTestItemCodeExcelColumnIndex; //Excel中测试项代号列序号
		QString strTestItemCodeExcelColumnName; //Excel中测试项代号列标题

		int nTestItemNameExcelColumnIndex; //Excel中测试项名称列序号
		QString strTestItemNameExcelColumnName; //Excel中测试项名称列标题
	
		bool isValid()
		{
			if (strExcelPath.isEmpty()
				|| nTestItemCodeExcelColumnIndex < 1
				|| nTestItemNameExcelColumnIndex < 1
				|| nTestItemCodeExcelColumnIndex == nTestItemNameExcelColumnIndex)
			{
				return false;
			}
			return true;
		}
		
	}TestItemExcelInfo, * pTestItemExcelInfo;
};
