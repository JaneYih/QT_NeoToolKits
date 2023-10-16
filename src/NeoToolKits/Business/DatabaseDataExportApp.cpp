#include "DatabaseDataExportApp.h"
#include <QMessageBox>
#include "ExcelDataUploadDbOperate.h"
#include "IniOperation.h"
#include "ExcelOperation.h"
#include <QThread>
#include <QProgressDialog>
#include <QtXlsx>
using namespace QXlsx;

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

//ini prefixs
const QString s_ini_prefix_sql = QString::fromStdWString(L"MySQL配置");
const QString s_ini_prefix_queryCondition = QString::fromStdWString(L"查询条件");

//ini keys
const QString s_ini_key_workOrderID = QString::fromStdWString(L"工单号");
const QString s_ini_key_boxNumStart = QString::fromStdWString(L"起始箱号");
const QString s_ini_key_boxNumEnd = QString::fromStdWString(L"末尾箱号");

DatabaseDataExportApp::DatabaseDataExportApp(QWidget* parent)
	: QWidget(parent)
	, m_strIniFileName(QCoreApplication::applicationDirPath() + "/DatabaseDataExportTool.ini")
	, m_pCfg(new IniOperation(m_strIniFileName))
{
	m_exportConfig.dataIndexCondition.workOrderID = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_workOrderID, "").toString();
	m_exportConfig.dataIndexCondition.BoxNumberStart = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_boxNumStart, "").toString();
	m_exportConfig.dataIndexCondition.BoxNumberEnd = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_boxNumEnd, "").toString();

	m_pWorkerThread = new QThread();
	m_pWorker = new DatabaseDataExportWorker();
	m_pWorker->moveToThread(m_pWorkerThread);
	connect(this, &DatabaseDataExportApp::toWork, m_pWorker, &DatabaseDataExportWorker::DoWork);
	m_pWorkerThread->start();
}

DatabaseDataExportApp::~DatabaseDataExportApp()
{
	delete m_pCfg;
}

ExportConfig DatabaseDataExportApp::getExportConfig() const
{
	return m_exportConfig;
}

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

bool DatabaseDataExportApp::StartExport(const ExportConfig& cfg)
{
	m_progressDialog = new QProgressDialog(this);
	m_progressDialog->setWindowTitle(QString::fromStdWString(L"导出数据"));
	m_progressDialog->setWindowModality(Qt::WindowModal);
	m_progressDialog->setLabelText(QString::fromStdWString(L"正在查询数据..."));
	m_progressDialog->setRange(0, 1);
	m_progressDialog->show();
	QCoreApplication::processEvents();

	connect(m_pWorker, &DatabaseDataExportWorker::setRange, m_progressDialog, &QProgressDialog::setRange);
	connect(m_pWorker, &DatabaseDataExportWorker::setValue, m_progressDialog, &QProgressDialog::setValue);
	connect(m_pWorker, &DatabaseDataExportWorker::setLabelText, m_progressDialog, &QProgressDialog::setLabelText);

	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_workOrderID, cfg.dataIndexCondition.workOrderID);
	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_boxNumStart, cfg.dataIndexCondition.BoxNumberStart);
	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_boxNumEnd, cfg.dataIndexCondition.BoxNumberEnd);
	m_exportConfig = cfg;
	emit toWork(this);
	return true;
}

#pragma region DatabaseDataExportWorker
void DatabaseDataExportWorker::DoWork(DatabaseDataExportApp* const& pApp)
{
	bool bExportSucceed = false;
	QString strErrorMsg;
	do
	{
		DatabaseDataExportDbOperate db(*(pApp->getSqlTableInfoPointer()));
		DataTable outputData;
		if (!db.QueryDataByIndexCondition(pApp->getExportConfig(), outputData, strErrorMsg))
		{
			break;
		}
		emit setValue(1);
		emit setLabelText(QString::fromStdWString(L"正在生成Excel..."));
		emit setRange(0, outputData.RowList.size());
		if (!SaveAsExcelSheet(pApp->getExportConfig(), outputData, pApp->m_progressDialog, strErrorMsg))
		{
			break;
		}
		bExportSucceed = true;
	} while (false);

	emit pApp->toExportFinish(bExportSucceed, strErrorMsg);
}

bool DatabaseDataExportWorker::SaveAsExcelSheet(const ExportConfig& queryCfg, const DataTable& data,
	QProgressDialog* progressDialog, QString& strErrorMsg)
{
	if (data.RowList.size() <= 0)
	{
		strErrorMsg = QString::fromStdWString(L"查询数据为空，无法导出数据！");
		return false;
	}

	QMap<QString, QString> dbKeyExcelTitleMap;
	for each (auto var in queryCfg.exportFields)
	{
		dbKeyExcelTitleMap[var.DbKey] = var.ExcelTitle;
	}

	QXlsx::Document xlsx;
	QXlsx::Format format1;
	format1.setFontBold(true);
	format1.setFontColor(QColor(Qt::black));
	format1.setBorderStyle(Format::BorderThin);
	format1.setFontSize(12);
	format1.setVerticalAlignment(Format::AlignVCenter);
	format1.setHorizontalAlignment(Format::AlignHCenter);

	//标题
	int ColumnsIndex = 1;
	int rowIndex = 1;
	for each (auto var in data.FieldName.FieldListValue)
	{
		QString dbKey = QString::fromStdString(var);
		QString strText(dbKey);
		if (dbKeyExcelTitleMap.find(dbKey) != dbKeyExcelTitleMap.end())
		{
			strText = dbKeyExcelTitleMap[dbKey];
		}
		if (!strText.isEmpty())
		{
			xlsx.setColumnWidth(ColumnsIndex, strText.length() + 5);
		}
		if (!xlsx.write(rowIndex, ColumnsIndex, strText, format1))
		{
			strErrorMsg = QString::fromStdWString(L"<%1行,%2列>标题写入失败").arg(rowIndex).arg(ColumnsIndex);
			return false;
		}
		ColumnsIndex++;
	}
	xlsx.setRowHeight(rowIndex, 25);

	//内容
	format1.setFontBold(false);
	format1.setFontColor(QColor(Qt::black));
	rowIndex = 2;
	for each (auto row in data.RowList)
	{
		ColumnsIndex = 1;
		for each (auto var in row.FieldListValue)
		{
			QString strText = QString::fromStdString(var);
			if (!strText.isEmpty())
			{
				int newColumnWidth = strText.length() + 5;
				int oldColumnWidth = xlsx.columnWidth(ColumnsIndex);
				if (newColumnWidth > oldColumnWidth)
				{
					xlsx.setColumnWidth(ColumnsIndex, newColumnWidth);
				}
			}
			if (!xlsx.write(rowIndex, ColumnsIndex, strText, format1))
			{
				strErrorMsg = QString::fromStdWString(L"<%1行,%2列>内容写入失败").arg(rowIndex).arg(ColumnsIndex);
				return false;
			}
			ColumnsIndex++;
		}

		emit setValue(rowIndex);

		xlsx.setRowHeight(rowIndex, 22);
		rowIndex++;

		if (progressDialog->wasCanceled())
		{
			strErrorMsg = QString::fromStdWString(L"用户取消操作。");
			return false;
		}
	}

	if (!xlsx.saveAs(queryCfg.excelName))
	{
		strErrorMsg = QString::fromStdWString(L"excel文件保存失败：%1").arg(queryCfg.excelName);
		return false;
	}
	return true;
}
#pragma endregion DatabaseDataExportWorker
