#pragma once
#include <QString>

#define NAMESPACENAME_EXCEL_DATA_UPLOAD  Namespace_ExcelDataUpload 

namespace NAMESPACENAME_EXCEL_DATA_UPLOAD
{
	typedef struct _ExcelDataUploadInfo_
	{
		int ExcelColumnIndex; //Excel数据列序号
		QString ExcelColumnName; //Excel数据列名称
		QString DbFieldName; //数据库字段名
		explicit _ExcelDataUploadInfo_(int excelColumnIndex, const QString& excelColumnName, const QString& dbFieldName = "")
			: ExcelColumnIndex(excelColumnIndex),
			ExcelColumnName(excelColumnName),
			DbFieldName(dbFieldName)
		{
		}
		bool isValid()
		{
			return !DbFieldName.isEmpty() && ExcelColumnIndex > 0;
		}

	}ExcelDataUploadInfo, * pExcelDataUploadInfo;

	typedef struct _ExcelDataUploadConfig_
	{
		void* pApp = nullptr;
		bool bErrorStop = false;
		int iRowCountMax = 0;
		int iRowCount = 0;
		int iRowStartRowIndex = 1;
		QString strProductionOrderID = "";
	}ExcelDataUploadConfig, * pExcelDataUploadConfig;

	typedef struct _UploadData_
	{
		QString key;   //数据库字段名
		QString value; //数据值
		_UploadData_(const QString& k, const QString& v)
			: key(k),
			value(v)
		{}
	}UploadData, * pUploadData;

	typedef struct _UploadingInfo_
	{
		volatile bool bStop;
		_UploadingInfo_()
		{
			clear();
		}
		void clear()
		{
			bStop = false;
		}
	}UploadingInfo, * pUploadingInfo;
};
