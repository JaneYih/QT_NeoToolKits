#pragma once
#include <QString>

#define NAMESPACENAME_DB_SCRIPT_EDITOR  Namespace_DbScriptEditor 

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	typedef struct _TestItemExcelInfo_
	{
		QString strExcelPath; //Excel·��
		QStringList listExcelColumns; //excel�ı����б�

		int nTestItemCodeExcelColumnIndex; //Excel�в�������������
		QString strTestItemCodeExcelColumnName; //Excel�в���������б���

		int nTestItemNameExcelColumnIndex; //Excel�в��������������
		QString strTestItemNameExcelColumnName; //Excel�в����������б���
	
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
