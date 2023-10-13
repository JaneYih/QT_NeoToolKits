#pragma once

#define NAMESPACENAME_DATABASE_DATA_EXPORT  Namespace_DatabaseDataExport


typedef struct _ExportDataUnit_
{
	bool bExport; //是否导出
	QString DbKey;   //数据库字段名
	QString ExcelTitle; //Excel标题名
	_ExportDataUnit_(const QString& k, const QString& v)
		: DbKey(k),
		ExcelTitle(v),
		bExport(false)
	{}
}ExportDataUnit, * pExportDataUnit;