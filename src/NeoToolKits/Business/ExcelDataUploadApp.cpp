#include "ExcelDataUploadApp.h"
#include <QMessageBox>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

//ini prefixs
const QString s_ini_prefix_excel = "Excel";

//ini keys
const QString s_ini_key_excelFile = "ExcelFlie";

ExcelDataUploadApp::ExcelDataUploadApp(QObject *parent)
	: QObject(parent),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/ExcelDataUploadTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName)),
	m_strExcelFileName(m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString())
{
	m_pMysqlInfoDlg = new MysqlInfoPopDialog(static_cast<QWidget*>(parent), false);
	m_pMysqlInfoDlg->setIniFileName(m_strIniFileName);
	m_pMysqlInfoDlg->setIniPrefix(QString::fromStdWString(L"MySQL����"));
	m_pMysqlInfoDlg->LoadIniCfg();
}

ExcelDataUploadApp::~ExcelDataUploadApp()
{
	delete m_pMysqlInfoDlg;
	delete m_pCfg;
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

MysqlInfoPopDialog* ExcelDataUploadApp::getMysqlInfoDlg() const
{
	return m_pMysqlInfoDlg;
}

QStringList ExcelDataUploadApp::LoadExcelColumns(const QString& fileName)
{
	QStringList strOutList;
	QXlsx::Document xlsx(fileName);
	if (xlsx.load())
	{
		QXlsx::Workbook* workBook = xlsx.workbook();
		QXlsx::Worksheet* workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(0));
		CellRange* cellRange = &(workSheet->dimension());
		int rowCount = cellRange->rowCount();
		int columnCount = cellRange->columnCount();
		for (int col = 1; col <= columnCount; col++)
		{
			Cell* cell = xlsx.cellAt(1, col);
			if (cell != NULL)
			{
				QVariant var = cell->readValue();
				strOutList.push_back(var.toString());
			}
		}
		setExcelFileName(fileName);
	}
	else
	{
		QMessageBox::critical(nullptr, "error", "excel open fail:" + fileName);
	}
	return strOutList;
}
