#pragma once

#define NAMESPACENAME_DATABASE_DATA_EXPORT  Namespace_DatabaseDataExport


typedef struct _ExportDataUnit_
{
	bool bExport; //�Ƿ񵼳�
	bool bAllEmptyUnexport; //�����ѯ�Ľ������ȫΪ�գ��򲻵�������
	QString DbKey;   //���ݿ��ֶ���
	QString ExcelTitle; //Excel������
	_ExportDataUnit_(const QString& key, const QString& name, bool onoff, bool emptyUnexport)
		: DbKey(key)
		, ExcelTitle(name)
		, bExport(onoff)
		, bAllEmptyUnexport(emptyUnexport)
	{}
}ExportDataUnit, * pExportDataUnit;

typedef struct _DatabaseDataIndexCondition_ 
{
	QString workOrderID; //������
	QString BoxNumberStart; //��ʼ���
	QString BoxNumberEnd;//ĩβ���
	QString TrayNoStart; //��ʼ���̺�
	QString TrayNoEnd;//ĩβ���̺�
	void clear()
	{
		workOrderID = "";
		BoxNumberStart = "";
		BoxNumberEnd = "";
		TrayNoStart = "";
		TrayNoEnd = "";
	}
}DatabaseDataIndexCondition, * pDatabaseDataIndexCondition;

typedef struct _ExportConfig_
{
	QString excelName;
	DatabaseDataIndexCondition dataIndexCondition;  //������������
	QList<ExportDataUnit> exportFields; //���������ֶ�
}ExportConfig, * pExportConfig;
