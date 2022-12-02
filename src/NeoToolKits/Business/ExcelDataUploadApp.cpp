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
const QString s_ini_prefix_sql = QString::fromStdWString(L"MySQLÅäÖÃ");

//ini keys
const QString s_ini_key_excelFile = "ExcelFlie";

ExcelDataUploadApp::ExcelDataUploadApp(QObject* parent)
	: QObject(parent),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/ExcelDataUploadTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName)),
	m_strExcelFileName(m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString())
{

}

ExcelDataUploadApp::~ExcelDataUploadApp()
{
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
