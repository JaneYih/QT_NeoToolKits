#pragma once

#include <QObject>
#include "Database_def.h"
#include "ExcelDataUpload_def.h"
#include "xlsxdocument.h"

class QThread;
class IniOperation;

namespace NAMESPACENAME_EXCEL_DATA_UPLOAD
{
	class ExcelDataUploadApp : public QObject
	{
		Q_OBJECT

	public:
		ExcelDataUploadApp(QObject* parent);
		~ExcelDataUploadApp();

		bool StartUpload();
		bool StopUpload();
		void PackingUploadDataList(QList<QVector<UploadData>>& dataList);

		QStringList LoadExcelColumns(const QString& fileName);
		int LoadExcelRowCount() {
			return LoadExcelRowCount(m_strExcelFileName);
		}

		QString getExcelFileName() const;
		void setExcelFileName(const QString& filename);

		QString getIniFileName() const;
		QString getIniSqlCfgPrefix() const;

		void setDataMap(const QVector<ExcelDataUploadInfo>& dataMap);
		const QVector<ExcelDataUploadInfo> getDataMap() const;

		void setUploadConfig(const ExcelDataUploadConfig& stUploadConfig);
		const ExcelDataUploadConfig getUploadConfig() const;

		bool getUploading() const;
		void setUploading(bool bUploading);

		SqlTableInfo* getSqlTableInfoPointer() const;
		UploadingInfo* getUploadingInfoPointer() const;

	private:
		QXlsx::CellRange OpenExcelSheet(const QXlsx::Document& xlsx, int index = 0);
		int LoadExcelRowCount(const QString& fileName);

	private:
		QString m_strIniFileName;
		IniOperation* m_pCfg;
		QString m_strExcelFileName;

		SqlTableInfo m_stTableInfo; //数据库配置信息
		QVector<ExcelDataUploadInfo> m_vecDataMap; //Excel信息与数据库字段关系表
		ExcelDataUploadConfig m_stUploadConfig; //上传配置信息
		
		QThread* m_pUploadWorkerThread; //上传工作线程
		UploadingInfo* m_pstUploadingInfo; //上传的运行时信息

	Q_SIGNALS:
		void toUploadWork(ExcelDataUploadApp* const& pApp);
		void toDisplayItem(const QString& text, int count, int countMax);
		void toDisplayFinish();
	};

	class ExcelDataUploadWorker : public QObject
	{
		Q_OBJECT
	public slots:
		void DoWork(ExcelDataUploadApp* const& pApp);
	};
};
