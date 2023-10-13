#include "DatabaseDataExportApp.h"
#include <QMessageBox>
#include "ExcelDataUploadDbOperate.h"
#include "IniOperation.h"
#include "ExcelOperation.h"
#include <QThread>
#include <QtXlsx>
using namespace QXlsx;

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

//ini prefixs
const QString s_ini_prefix_excel = "Excel";
const QString s_ini_prefix_sql = QString::fromStdWString(L"MySQL配置");

//ini keys
//const QString s_ini_key_excelFile = "ExcelFlie";

DatabaseDataExportApp::DatabaseDataExportApp(QObject* parent)
	: QObject(parent),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/DatabaseDataExportTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName))//,
	//m_strExcelFileName(m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString()),
	//m_pstUploadingInfo(new UploadingInfo())
{
	//Q_ASSERT(m_pstUploadingInfo);
	//m_pUploadWorkerThread = new QThread();
	//ExcelDataUploadWorker* uploadWorkerWorker = new ExcelDataUploadWorker();
	//uploadWorkerWorker->moveToThread(m_pUploadWorkerThread);
	//connect(this, &DatabaseDataExportApp::toUploadWork, uploadWorkerWorker, &ExcelDataUploadWorker::DoWork);
	//m_pUploadWorkerThread->start();
}

DatabaseDataExportApp::~DatabaseDataExportApp()
{
	//delete m_pstUploadingInfo;
	delete m_pCfg;
}

//void DatabaseDataExportApp::setDataMap(const QVector<ExcelDataUploadInfo>& dataMap)
//{
//	m_vecDataMap = dataMap;
//}
//
//const QVector<ExcelDataUploadInfo> DatabaseDataExportApp::getDataMap() const
//{
//	return m_vecDataMap;
//}
//
//void DatabaseDataExportApp::setExcelFileName(const QString& filename)
//{
//	m_strExcelFileName = filename;
//	m_pCfg->WriteValue(s_ini_prefix_excel, s_ini_key_excelFile, filename);
//}
//
//QString DatabaseDataExportApp::getExcelFileName() const
//{
//	return m_strExcelFileName;
//}
//
QString DatabaseDataExportApp::getIniFileName() const
{
	return m_strIniFileName;
}

QString DatabaseDataExportApp::getIniSqlCfgPrefix() const
{
	return s_ini_prefix_sql;
}

SqlTableInfo* DatabaseDataExportApp::getSqlTableInfoPointer() const
{
	return (SqlTableInfo*)(&m_stTableInfo);
}

//void DatabaseDataExportApp::setUploadConfig(const ExcelDataUploadConfig& stUploadConfig)
//{
//	m_stUploadConfig = stUploadConfig;
//}
//
//const ExcelDataUploadConfig DatabaseDataExportApp::getUploadConfig() const
//{
//	return m_stUploadConfig;
//}
//
//bool DatabaseDataExportApp::getUploading() const 
//{
//	return m_pstUploadingInfo->bUploading;
//};
//
//void DatabaseDataExportApp::setUploading(bool bUploading)
//{
//	m_pstUploadingInfo->bUploading = bUploading;
//};
//
//UploadingInfo* DatabaseDataExportApp::getUploadingInfoPointer() const
//{
//	return m_pstUploadingInfo;
//}
//
//QStringList DatabaseDataExportApp::LoadExcelColumns(const QString& fileName)
//{
//	return ExcelOperation::LoadExcelColumns(fileName);
//}
//
//int DatabaseDataExportApp::LoadExcelRowCount(const QString& fileName)
//{
//	return ExcelOperation::LoadExcelRowCount(fileName);
//}
//
//bool DatabaseDataExportApp::StartUpload()
//{
//	m_pstUploadingInfo->clear();
//	emit toUploadWork(this);
//	return true;
//}
//
//
//void ExcelDataUploadWorker::DoWork(DatabaseDataExportApp* const& pApp)
//{
//	pApp->setUploading(true);
//
//	emit pApp->toDisplayItem(QString::fromStdWString(L"正在玩命加载Excel内容,请耐心等候...."), 0, 0);
//	QList<QVector<UploadData>> dataList;
//	pApp->PackingUploadDataList(dataList);
//	emit pApp->toDisplayItem(QString::fromStdWString(L"Excel内容加载完毕。"), 0, dataList.count());
//
//	if (dataList.count() == pApp->getUploadConfig().iRowCount)
//	{
//		ExcelDataUploadDbOperate db(*(pApp->getSqlTableInfoPointer()));
//		if (pApp->getUploadConfig().eOpentions == UploadOptions::InsertCommand)
//		{
//			db.InsertExcelData(dataList, pApp->getUploadConfig(), pApp->getUploadingInfoPointer());
//		}
//		else if (pApp->getUploadConfig().eOpentions == UploadOptions::UpdateCommand_EmptyFill)
//		{
//			db.UpdateExcelData_EmptyFill(dataList, pApp->getUploadConfig(), pApp->getUploadingInfoPointer());
//		}
//		else if (pApp->getUploadConfig().eOpentions == UploadOptions::UpdateCommand_Rewrite)
//		{
//			db.UpdateExcelData_Rewrite(dataList, pApp->getUploadConfig(), pApp->getUploadingInfoPointer());
//		}
//	}
//	else
//	{
//		emit pApp->toDisplayItem(QString::fromStdWString(L"整理的数据数量(%1)与期望(%2)不一致")
//								.arg(dataList.count()).arg(pApp->getUploadConfig().iRowCount), 
//								0, dataList.count());
//	}
//
//	pApp->setUploading(false);
//	emit pApp->toDisplayFinish();
//}
//
