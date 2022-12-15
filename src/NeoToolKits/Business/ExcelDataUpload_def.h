#pragma once
#include <QString>

#define NAMESPACENAME_EXCEL_DATA_UPLOAD  Namespace_ExcelDataUpload 

namespace NAMESPACENAME_EXCEL_DATA_UPLOAD
{
	typedef struct _ExcelDataUploadInfo_
	{
		int ExcelColumnIndex; //Excel���������
		QString ExcelColumnName; //Excel����������
		QString DbFieldName; //���ݿ��ֶ���
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
				msg = QString::fromStdWString(L"�����Ų���Ϊ��");
				return false;
			}
			if (strProductionOrderID.length() < 5)
			{
				msg = QString::fromStdWString(L"�����ų��Ȳ���С��5");
				return false;
			}
			if (iRowCountMax < 1)
			{
				msg = QString::fromStdWString(L"�����������С��1");
				return false;
			}
			if (iRowCount < 1)
			{
				msg = QString::fromStdWString(L"��������С��1");
				return false;
			}
			if (iRowStartRowIndex < 2)
			{
				msg = QString::fromStdWString(L"��ʼ�в���С��2");
				return false;
			}
			if (pApp == nullptr)
			{
				msg = QString::fromStdWString(L"appʵ������Ϊnullptr");
				return false;
			}
			return true;
		}
	}ExcelDataUploadConfig, * pExcelDataUploadConfig;

	typedef struct _UploadData_
	{
		QString key;   //���ݿ��ֶ���
		QString value; //����ֵ
		_UploadData_(const QString& k, const QString& v)
			: key(k),
			value(v)
		{}
	}UploadData, * pUploadData;

	typedef struct _UploadingInfo_
	{
		volatile bool bStop; //�����ź�-��ֹ�ϴ���־
		volatile bool bUploading;  //״̬�ź�-�����ϴ���־
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
