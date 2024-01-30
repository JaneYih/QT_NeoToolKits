#include "DatabaseDataExportApp.h"
#include <QMessageBox>
#include "ExcelDataUploadDbOperate.h"
#include "IniOperation.h"
#include "ExcelOperation.h"
#include "msgBox.h"
#include <QThread>
#include <QProgressDialog>
#include <QtXlsx>
#include <QFileInfo>
using namespace QXlsx;

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

//ini prefixs
const QString s_ini_prefix_sql = QString::fromStdWString(L"MySQL����");
const QString s_ini_prefix_queryCondition = QString::fromStdWString(L"��ѯ����");

//ini keys
const QString s_ini_key_workOrderID = QString::fromStdWString(L"������");
const QString s_ini_key_boxNumStart = QString::fromStdWString(L"��ʼ���");
const QString s_ini_key_boxNumEnd = QString::fromStdWString(L"ĩβ���");
const QString s_ini_key_trayNoStart = QString::fromStdWString(L"��ʼ���̺�");
const QString s_ini_key_trayNoEnd = QString::fromStdWString(L"ĩβ���̺�");

DatabaseDataExportApp::DatabaseDataExportApp(QWidget* parent)
	: QWidget(parent)
	, m_strIniFileName(QCoreApplication::applicationDirPath() + "/DatabaseDataExportTool.ini")
	, m_pCfg(new IniOperation(m_strIniFileName))
{
	m_exportConfig.dataIndexCondition.workOrderID = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_workOrderID, "").toString();
	m_exportConfig.dataIndexCondition.BoxNumberStart = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_boxNumStart, "").toString();
	m_exportConfig.dataIndexCondition.BoxNumberEnd = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_boxNumEnd, "").toString();
	m_exportConfig.dataIndexCondition.TrayNoStart = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_trayNoStart, "").toString();
	m_exportConfig.dataIndexCondition.TrayNoEnd = m_pCfg->ReadValue(s_ini_prefix_queryCondition, s_ini_key_trayNoEnd, "").toString();

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
	m_progressDialog->setWindowTitle(QString::fromStdWString(L"��������"));
	m_progressDialog->setWindowModality(Qt::WindowModal);
	m_progressDialog->setLabelText(QString::fromStdWString(L"���ڲ�ѯ����..."));
	m_progressDialog->setAutoClose(true);
	m_progressDialog->show();
	QCoreApplication::processEvents();

	connect(m_pWorker, &DatabaseDataExportWorker::toSetRange, m_progressDialog, &QProgressDialog::setRange);
	connect(m_pWorker, &DatabaseDataExportWorker::toSetValue, m_progressDialog, &QProgressDialog::setValue);
	connect(m_pWorker, &DatabaseDataExportWorker::toSetLabelText, m_progressDialog, &QProgressDialog::setLabelText);

	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_workOrderID, cfg.dataIndexCondition.workOrderID);
	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_boxNumStart, cfg.dataIndexCondition.BoxNumberStart);
	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_boxNumEnd, cfg.dataIndexCondition.BoxNumberEnd);
	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_trayNoStart, cfg.dataIndexCondition.TrayNoStart);
	m_pCfg->WriteValue(s_ini_prefix_queryCondition, s_ini_key_trayNoEnd, cfg.dataIndexCondition.TrayNoEnd);
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

		/*auto useUserSelectBtn = msgBox::show("Question", QString::fromStdWString(L"��������%1�����ݣ������Ƿ������").arg(outputData.RowList.size()), msgBox::question);
		if (useUserSelectBtn != QMessageBox::Yes)
		{
			strErrorMsg = QString::fromStdWString(L"�û�ȡ��������");
			break;
		}*/

		emit toSetLabelText(QString::fromStdWString(L"���ڹ���ȫ����..."));
		RemoveAllEmptyFieldData(pApp->getExportConfig(), outputData);

		emit toSetLabelText(QString::fromStdWString(L"��������Excel..."));
		emit toSetRange(1, outputData.RowList.size()+2);
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
		strErrorMsg = QString::fromStdWString(L"��ѯ����Ϊ�գ��޷��������ݣ�");
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

	//����
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
			strErrorMsg = QString::fromStdWString(L"<%1��,%2��>����д��ʧ��").arg(rowIndex).arg(ColumnsIndex);
			return false;
		}
		ColumnsIndex++;
	}
	xlsx.setRowHeight(rowIndex, 25);
	int ProgressValue = 1;
	emit toSetValue(ProgressValue++);

	//����
	format1.setFontBold(false);
	format1.setFontColor(QColor(Qt::black));
	rowIndex = 2;
	int iDataIndex = 0;
	int progressOffset = 5; //ÿ��5%�͸���һ�½�����
	int lastPercentage = -progressOffset;
	for each (auto row in data.RowList)
	{
		ColumnsIndex = 1;
		for each (auto var in row.FieldListValue)
		{
			QString strText = QString::fromLocal8Bit(var.c_str());
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
				strErrorMsg = QString::fromStdWString(L"<%1��,%2��>����д��ʧ��").arg(rowIndex).arg(ColumnsIndex);
				return false;
			}
			ColumnsIndex++;
		}

		xlsx.setRowHeight(rowIndex, 22);
		int percentage = (++iDataIndex * 1.0 / data.RowList.size()) * 100;
		if (percentage % progressOffset == 0
			&& percentage > lastPercentage) //ÿ��5%�͸���һ�½���
		{
			emit toSetValue(ProgressValue); //��ѭ�����������ûᵼ�¶�ջ�ڴ治�����
			emit toSetLabelText(QString::fromStdWString(L"��������Excel��%1/%2��...").arg(iDataIndex).arg(data.RowList.size()));
			lastPercentage += progressOffset;
		}
		ProgressValue++;
		rowIndex++;
		if (progressDialog->wasCanceled())
		{
			strErrorMsg = QString::fromStdWString(L"�û�ȡ��������");
			return false;
		}
	}

	QFileInfo fileInfo(queryCfg.excelName);
	QString fileFullName = QString("%1/%2[%3].%4")
		.arg(fileInfo.absoluteDir().absolutePath())
		.arg(fileInfo.baseName())
		.arg(data.RowList.size())
		.arg(fileInfo.suffix());
	if (!xlsx.saveAs(fileFullName))
	{
		strErrorMsg = QString::fromStdWString(L"excel�ļ�����ʧ�ܣ�%1").arg(fileFullName);
		return false;
	}
	emit toSetValue(ProgressValue++);

	//�Զ�����ȱʡ�ĵ���Ӧ��������ļ�
	//bool bOpenUrlSucceed = QDesktopServices::openUrl(QUrl(fileFullName, QUrl::TolerantMode)); 
	
	return true;
}

void DatabaseDataExportWorker::RemoveAllEmptyFieldData(const ExportConfig& queryCfg, DataTable& data)
{
	QMap<QString, bool> dbKeyEmptyUnexportMap;
	for each (auto var in queryCfg.exportFields)
	{
		if (var.bAllEmptyUnexport)
		{
			dbKeyEmptyUnexportMap[var.DbKey] = var.bAllEmptyUnexport;
		}
	}
	if (dbKeyEmptyUnexportMap.isEmpty())
	{
		return;
	}

	QVector<int> allEmptyColIndexs;
	int colIndex = 0;
	for each (auto var in data.FieldName.FieldListValue) //ÿһ��
	{
		QString dbKey = QString::fromStdString(var);
		if (dbKeyEmptyUnexportMap.find(dbKey) != dbKeyEmptyUnexportMap.end())
		{
			if (dbKeyEmptyUnexportMap[dbKey])
			{
				bool isAllEmpty = true;
				for each (auto row in data.RowList) 
				{
					int cols = 0;
					for each (auto var in row.FieldListValue)  
					{
						if (cols == colIndex) //��һ�е�ÿһ������
						{
							QString dbValue = QString::fromLocal8Bit(var.c_str());
							if (!dbValue.isEmpty())
							{
								isAllEmpty = false;
								break;
							}
						}
						cols++;
					}
					if (!isAllEmpty)
					{
						break;
					}
				}
				if (isAllEmpty)
				{
					allEmptyColIndexs.push_back(colIndex);
				}
			}
		}
		colIndex++;
	}

	//�Ƴ�����
	FieldList NewFieldName;
	int i = 0;
	for each (auto var in data.FieldName.FieldListValue)
	{
		if (!allEmptyColIndexs.contains(i++))
		{
			NewFieldName.FieldListValue.push_back(var);
		}
	}
	data.FieldName = NewFieldName;

	//�Ƴ�����
	vector<FieldList> NewRowList;
	for each (auto row in data.RowList)
	{
		FieldList newRow;
		int cols = 0;
		for each (auto var in row.FieldListValue)
		{
			if (!allEmptyColIndexs.contains(cols++))
			{
				newRow.FieldListValue.push_back(var);
			}
		}
		NewRowList.push_back(newRow);
	}
	data.RowList = NewRowList;
}

#pragma endregion DatabaseDataExportWorker
