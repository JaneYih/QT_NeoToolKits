#pragma once

#include <QWidget>
#include "Database_def.h"
#include "DatabaseDataExport_def.h"
#include "DatabaseDataExportDbOperate.h"


class QThread;
class IniOperation;
class QProgressDialog;

namespace NAMESPACENAME_DATABASE_DATA_EXPORT
{
	class DatabaseDataExportWorker;

	class DatabaseDataExportApp : public QWidget
	{
		Q_OBJECT

	public:
		DatabaseDataExportApp(QWidget* parent);
		~DatabaseDataExportApp();

		bool StartExport(const ExportConfig& cfg);

		QString getIniFileName() const;
		QString getIniSqlCfgPrefix() const;
		SqlTableInfo* getSqlTableInfoPointer() const;
		ExportConfig getExportConfig() const;

	public:
		QProgressDialog* m_progressDialog;
		
	private:
		QString m_strIniFileName;
		IniOperation* m_pCfg;

		ExportConfig m_exportConfig;
		SqlTableInfo m_stTableInfo; //数据库配置信息
		QThread* m_pWorkerThread; //工作线程
		DatabaseDataExportWorker* m_pWorker;

	Q_SIGNALS:
		void toWork(DatabaseDataExportApp* const& pApp);
		void toExportFinish(bool result, const QString& errorMsg);
	};

	class DatabaseDataExportWorker : public QObject
	{
		Q_OBJECT

	Q_SIGNALS:
		void toSetRange(int minimum, int maximum);
		void toSetValue(int progress);
		void toSetLabelText(const QString& text);

	public slots:
		void DoWork(DatabaseDataExportApp* const& pApp);

	private:
		bool SaveAsExcelSheet(const ExportConfig& queryCfg, const DataTable& data,
			QProgressDialog* progressDialog, QString& strErrorMsg);
		void RemoveAllEmptyFieldData(const ExportConfig& queryCfg, DataTable& data);
	};
};
