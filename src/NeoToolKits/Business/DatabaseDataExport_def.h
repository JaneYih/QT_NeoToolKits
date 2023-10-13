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