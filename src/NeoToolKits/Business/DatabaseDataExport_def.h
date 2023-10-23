#pragma once

#define NAMESPACENAME_DATABASE_DATA_EXPORT  Namespace_DatabaseDataExport


typedef struct _ExportDataUnit_
{
	bool bExport; //是否导出
	QString DbKey;   //数据库字段名
	QString ExcelTitle; //Excel标题名
	_ExportDataUnit_(const QString& key, const QString& name, bool onoff = false)
		: DbKey(key)
		, ExcelTitle(name)
		, bExport(onoff)
	{}
}ExportDataUnit, * pExportDataUnit;

typedef struct _DatabaseDataIndexCondition_ 
{
	QString workOrderID; //工单号
	QString BoxNumberStart; //起始箱号
	QString BoxNumberEnd;//末尾箱号
	QString TrayNoStart; //起始托盘号
	QString TrayNoEnd;//末尾托盘号
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
	DatabaseDataIndexCondition dataIndexCondition;  //数据索引条件
	QList<ExportDataUnit> exportFields; //导出数据字段
}ExportConfig, * pExportConfig;
