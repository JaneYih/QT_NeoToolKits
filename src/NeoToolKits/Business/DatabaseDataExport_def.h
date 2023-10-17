#pragma once

#define NAMESPACENAME_DATABASE_DATA_EXPORT  Namespace_DatabaseDataExport


typedef struct _ExportDataUnit_
{
	bool bExport; //�Ƿ񵼳�
	QString DbKey;   //���ݿ��ֶ���
	QString ExcelTitle; //Excel������
	_ExportDataUnit_(const QString& key, const QString& name, bool onoff = false)
		: DbKey(key)
		, ExcelTitle(name)
		, bExport(onoff)
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
