#pragma once
#include <QString>

typedef struct _ExcelDataUploadInfo_
{
	QString ExcelColumnName; //Excel����������
	QString DbFieldName; //���ݿ��ֶ���
	explicit _ExcelDataUploadInfo_(const QString& excelColumnName, const QString& dbFieldName = "")
		: ExcelColumnName(excelColumnName),
		DbFieldName(dbFieldName)
	{
	}
}ExcelDataUploadInfo, * pExcelDataUploadInfo;

typedef struct _ExcelDataUploadConfig_
{
	bool bErrorStop = false;
	int iRowCountMax = 0;
	int iRowCount = 0;
	int iRowStartRowIndex = 1;
	QString strProductionOrderID = "";
}ExcelDataUploadConfig, * pExcelDataUploadConfig;