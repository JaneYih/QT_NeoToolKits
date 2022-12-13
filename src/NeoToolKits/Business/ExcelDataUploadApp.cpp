#include "ExcelDataUploadApp.h"
#include <QMessageBox>
#include "ExcelDataUploadDbOperate.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

//ini prefixs
const QString s_ini_prefix_excel = "Excel";
const QString s_ini_prefix_sql = QString::fromStdWString(L"MySQL配置");

//ini keys
const QString s_ini_key_excelFile = "ExcelFlie";

ExcelDataUploadApp::ExcelDataUploadApp(QObject* parent)
	: QObject(parent),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/ExcelDataUploadTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName)),
	m_strExcelFileName(m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString()),
	m_pstUploadingInfo(new UploadingInfo()),
	m_bUploading(false)
{
	m_pUploadWorkerThread = new QThread();
	ExcelDataUploadWorker* uploadWorkerWorker = new ExcelDataUploadWorker();
	uploadWorkerWorker->moveToThread(m_pUploadWorkerThread);
	connect(this, &ExcelDataUploadApp::toUploadWork, uploadWorkerWorker, &ExcelDataUploadWorker::DoWork);
	m_pUploadWorkerThread->start();
}

ExcelDataUploadApp::~ExcelDataUploadApp()
{
	delete m_pCfg;
}

void ExcelDataUploadApp::setDataMap(const QVector<ExcelDataUploadInfo>& dataMap)
{
	m_vecDataMap = dataMap;
}

const QVector<ExcelDataUploadInfo> ExcelDataUploadApp::getDataMap() const
{
	return m_vecDataMap;
}

void ExcelDataUploadApp::setExcelFileName(const QString& filename)
{
	m_strExcelFileName = filename;
	m_pCfg->WriteValue(s_ini_prefix_excel, s_ini_key_excelFile, filename);
}

QString ExcelDataUploadApp::getExcelFileName() const
{
	return m_strExcelFileName;
}

QString ExcelDataUploadApp::getIniFileName() const
{
	return m_strIniFileName;
}

QString ExcelDataUploadApp::getIniSqlCfgPrefix() const
{
	return s_ini_prefix_sql;
}

SqlTableInfo* ExcelDataUploadApp::getSqlTableInfoPointer() const
{
	return (SqlTableInfo*)(&m_stTableInfo);
}

void ExcelDataUploadApp::setUploadConfig(const ExcelDataUploadConfig& stUploadConfig)
{
	m_stUploadConfig = stUploadConfig;
}

const ExcelDataUploadConfig ExcelDataUploadApp::getUploadConfig() const
{
	return m_stUploadConfig;
}

bool ExcelDataUploadApp::getUploading() const 
{
	return m_bUploading;
};

void ExcelDataUploadApp::setUploading(bool bUploading)
{
	m_bUploading = bUploading;
};

const volatile bool& ExcelDataUploadApp::getStopUploadReference() const
{
	return m_bStopUpload;
}

UploadingInfo* ExcelDataUploadApp::getUploadingInfoPointer() const
{
	return m_pstUploadingInfo;
}

CellRange ExcelDataUploadApp::OpenExcelSheet(const QXlsx::Document& xlsx, int index)
{
	if (xlsx.load())
	{
		QXlsx::Workbook* workBook = xlsx.workbook();
		if (workBook)
		{
			QXlsx::Worksheet* workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(index));
			if (workSheet)
			{
				return workSheet->dimension();
			}
		}
	}
	else
	{
		//	QMessageBox::critical(nullptr, "error", "excel open fail:" + fileName);
	}
	return CellRange();
}

QStringList ExcelDataUploadApp::LoadExcelColumns(const QString& fileName)
{
	QStringList strOutList;
	QXlsx::Document xlsx(fileName);
	CellRange cellRange = OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		int rowCount = cellRange.rowCount();
		int columnCount = cellRange.columnCount();
		for (int col = 1; col <= columnCount; col++)
		{
			Cell* cell = xlsx.cellAt(1, col);
			if (cell != NULL)
			{
				QVariant var = cell->readValue();
				strOutList.push_back(var.toString());
			}
		}
	}
	return strOutList;
}

int ExcelDataUploadApp::LoadExcelRowCount(const QString& fileName)
{
	QXlsx::Document xlsx(fileName);
	CellRange cellRange = OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		return cellRange.rowCount();
	}
	return 0;
}

bool ExcelDataUploadApp::StartUpload()
{
	m_pstUploadingInfo->clear();
	m_stUploadConfig.pApp = (void*)(this);
	emit toUploadWork(this);
	return true;
}

bool ExcelDataUploadApp::StopUpload()
{
	m_pstUploadingInfo->bStop = true;
	return true;
}

void ExcelDataUploadApp::PackingUploadDataList(QList<QVector<UploadData>>& dataList)
{
	dataList.clear();
	QXlsx::Document xlsx(getExcelFileName());
	CellRange cellRange = OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		int rowCount = cellRange.rowCount();
		int columnCount = cellRange.columnCount();

		if (rowCount < m_stUploadConfig.iRowCount)
		{
			return;
		}

		int StartRowIndex = m_stUploadConfig.iRowStartRowIndex;
		for (int row = StartRowIndex; row < StartRowIndex + m_stUploadConfig.iRowCount; ++row)
		{
			QVector<UploadData> rowData;
			foreach (auto var, m_vecDataMap)
			{
				if (var.isValid())
				{
					Cell* cell = xlsx.cellAt(row, var.ExcelColumnIndex);
					if (cell != NULL)
					{
						rowData.push_back(UploadData(var.DbFieldName, cell->readValue().toString()));
					}
					else
					{
						rowData.push_back(UploadData(var.DbFieldName, ""));
					}
				}
			}
			dataList.push_back(rowData);
		}
	}
}

void ExcelDataUploadWorker::DoWork(ExcelDataUploadApp* const& pApp)
{
	pApp->setUploading(true);

	emit pApp->toDisplayItem(QString::fromStdWString(L"正在玩命加载Excel内容,请耐心等候...."), 0, 0);
	QList<QVector<UploadData>> dataList;
	pApp->PackingUploadDataList(dataList);
	emit pApp->toDisplayItem(QString::fromStdWString(L"Excel内容完毕。"), 0, dataList.count());
	if (dataList.count() == pApp->getUploadConfig().iRowCount)
	{
		ExcelDataUploadDbOperate db(*(pApp->getSqlTableInfoPointer()));
		db.UpdateExcelData(dataList, pApp->getUploadConfig(), pApp->getUploadingInfoPointer());
	}
	else
	{
		emit pApp->toDisplayItem(QString::fromStdWString(L"整理的数据数量(%1)与期望(%2)不一致")
								.arg(dataList.count()).arg(pApp->getUploadConfig().iRowCount), 
								0, dataList.count());
	}

	pApp->setUploading(false);
	emit pApp->toDisplayFinish();
}

