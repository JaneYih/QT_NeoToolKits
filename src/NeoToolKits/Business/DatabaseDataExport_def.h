#pragma once

#define NAMESPACENAME_DATABASE_DATA_EXPORT  Namespace_DatabaseDataExport


typedef struct _ExportDataUnit_
{
	bool bExport; //�Ƿ񵼳�
	QString DbKey;   //���ݿ��ֶ���
	QString ExcelTitle; //Excel������
	_ExportDataUnit_(const QString& k, const QString& v)
		: DbKey(k),
		ExcelTitle(v),
		bExport(false)
	{}
}ExportDataUnit, * pExportDataUnit;

typedef struct _DatabaseDataIndexCondition_ 
{
	QString workOrderID; //������
	QString BoxNumberStart; //��ʼ���
	QString BoxNumberEnd;//ĩβ���
	void clear()
	{
		workOrderID = "";
		BoxNumberStart = "";
		BoxNumberEnd = "";
	}
}DatabaseDataIndexCondition, * pDatabaseDataIndexCondition;

typedef struct _ExportConfig_
{
	QString excelName;
	DatabaseDataIndexCondition dataIndexCondition;  //������������
	QList<ExportDataUnit> exportFields; //���������ֶ�
}ExportConfig, * pExportConfig;
