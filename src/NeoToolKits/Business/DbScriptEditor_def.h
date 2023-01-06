#pragma once
#include <QString>

#define NAMESPACENAME_DB_SCRIPT_EDITOR  Namespace_DbScriptEditor 

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	typedef struct _TestItemExcelInfo_
	{
		QString strExcelPath; //Excel路径
		QStringList listExcelColumns; //excel的标题列表

		int nColIndex_ItemCode; //Excel中测试项代号列序号
		QString strColName_ItemCode; //Excel中测试项代号列标题

		int nColIndex_ItemName; //Excel中测试项名称列序号
		QString strColName_ItemName; //Excel中测试项名称列标题
	
		bool isValid()
		{
			if (strExcelPath.isEmpty()
				|| nColIndex_ItemCode < 1
				|| nColIndex_ItemName < 1
				|| nColIndex_ItemCode == nColIndex_ItemName)
			{
				return false;
			}
			return true;
		}
		
	}TestItemExcelInfo, * pTestItemExcelInfo;
};
