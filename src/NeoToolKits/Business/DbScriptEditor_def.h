#pragma once
#include <QString>

#define NAMESPACENAME_DB_SCRIPT_EDITOR  Namespace_DbScriptEditor 

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	typedef struct _TestItemExcelInfo_
	{
		QString strExcelPath; //Excel·��
		QStringList listExcelColumns; //excel�ı����б�

		int nColIndex_ItemCode; //Excel�в�������������
		QString strColName_ItemCode; //Excel�в���������б���

		int nColIndex_ItemName; //Excel�в��������������
		QString strColName_ItemName; //Excel�в����������б���
	
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
