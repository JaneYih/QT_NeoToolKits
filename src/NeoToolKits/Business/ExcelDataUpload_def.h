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
		int iRowStartRowIndex = 2;
		QString strProductionOrderID = "";
		bool isValid(QString& msg)
		{
			if (strProductionOrderID.isEmpty())
			{
				msg = QString::fromStdWString(L"工单号不能为空");
				return false;
			}
			if (strProductionOrderID.length() < 5)
			{
				msg = QString::fromStdWString(L"工单号长度不能小于5");
				return false;
			}
			if (iRowCountMax < 1)
			{
				msg = QString::fromStdWString(L"最大数量不能小于1");
				return false;
			}
			if (iRowCount < 1)
			{
				msg = QString::fromStdWString(L"数量不能小于1");
				return false;
			}
			if (iRowStartRowIndex < 2)
			{
				msg = QString::fromStdWString(L"起始行不能小于2");
				return false;
			}
			if (pApp == nullptr)
			{
				msg = QString::fromStdWString(L"app实例不能为nullptr");
				return false;
			}
			return true;
		}
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
		volatile bool bStop; //控制信号-终止上传标志
		volatile bool bUploading;  //状态信号-正在上传标志
		_UploadingInfo_()
		{
			clear();
		}
		void clear()
		{
			bStop = false;
			bUploading = false;
		}
	}UploadingInfo, * pUploadingInfo;
};
