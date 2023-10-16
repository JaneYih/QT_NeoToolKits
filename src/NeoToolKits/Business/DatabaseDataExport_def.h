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

typedef struct _DatabaseDataIndexCondition_ 
{
	QString workOrderID; //工单号
	QString BoxNumberStart; //起始箱号
	QString BoxNumberEnd;//末尾箱号
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
	DatabaseDataIndexCondition dataIndexCondition;  //数据索引条件
	QList<ExportDataUnit> exportFields; //导出数据字段
}ExportConfig, * pExportConfig;
