#include "DbScriptEditorApp.h"
#include "Database_def.h"
#include "DataTableTest.h"
#include "ExcelOperation.h"
#include "IniOperation.h"
#include <QMessageBox>
using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

//ini prefixs
const QString s_ini_prefix_excel = "Excel";

//ini keys
const QString s_ini_key_excelFile = "ExcelFlie";
const QString s_ini_key_ColumnIndex_TestItemCode = "ColumnIndex_TestItemCode";
const QString s_ini_key_ColumnIndex_TestItemName = "ColumnIndex_TestItemName";

DbScriptEditorApp::DbScriptEditorApp(QObject *parent)
	: QObject(parent),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/DbScriptEditorTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName))
{
	m_stTestItemExcelInfo.strExcelPath = m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString();
	m_stTestItemExcelInfo.nColIndex_ItemCode = m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_ColumnIndex_TestItemCode, -1).toInt();
	m_stTestItemExcelInfo.nColIndex_ItemName = m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_ColumnIndex_TestItemName, -1).toInt();
}

DbScriptEditorApp::~DbScriptEditorApp()
{
	delete m_pCfg;
}

QMap<QString, QString> DbScriptEditorApp::getTestItemDictionary() const
{
	return m_mapTestItemDictionary;
}

TestItemExcelInfo DbScriptEditorApp::getTestItemExcelInfo() const
{
	return m_stTestItemExcelInfo;
}

void DbScriptEditorApp::setTestItemExcelInfo(const TestItemExcelInfo& src)
{
	m_stTestItemExcelInfo = src;
	m_pCfg->WriteValue(s_ini_prefix_excel, s_ini_key_excelFile, src.strExcelPath);
	m_pCfg->WriteValue(s_ini_prefix_excel, s_ini_key_ColumnIndex_TestItemCode, src.nColIndex_ItemCode);
	m_pCfg->WriteValue(s_ini_prefix_excel, s_ini_key_ColumnIndex_TestItemName, src.nColIndex_ItemName);
}

QString DbScriptEditorApp::getSQLiteDbPath() const
{
	return m_strSQLiteDbPath;
}

void DbScriptEditorApp::setSQLiteDbPath(const QString& src)
{
	m_strSQLiteDbPath = src;
}

bool DbScriptEditorApp::TestSqliteDb(const QString& dbPath, const QString& tableName)
{
	SqlTableInfo tempInfo;
	tempInfo.baseInfo.type = SqlTypes::eSQLITE;
	tempInfo.baseInfo.dbName = dbPath;
	CDataTableTest db(tempInfo.baseInfo);
	//QString tableName("model_testlist");
	QByteArray byteTableName = tableName.toLocal8Bit();
	if (db.IsExistTable(byteTableName))
	{
		std::list<std::string> Fields;
		db.GetTableFullFields(byteTableName, Fields);
		return Fields.size() > 0;
	}
	return false;
}

void DbScriptEditorApp::LoadExcelColumns(const QString& fileName)
{
	m_stTestItemExcelInfo.strExcelPath = fileName;
	m_stTestItemExcelInfo.listExcelColumns = ExcelOperation::LoadExcelColumns(fileName);
}

bool DbScriptEditorApp::LoadExcelTestItemDictionary()
{
	using namespace QXlsx;
	m_mapTestItemDictionary.clear();

	if (!m_stTestItemExcelInfo.isValid())
	{
		return false;
	}

	Document xlsx(m_stTestItemExcelInfo.strExcelPath);
	CellRange cellRange = ExcelOperation::OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		int rowCount = cellRange.rowCount();
		int columnCount = cellRange.columnCount();

		int StartRowIndex = 2;
		for (int row = StartRowIndex; row < rowCount; ++row)
		{
			Cell* cell = xlsx.cellAt(row, m_stTestItemExcelInfo.nColIndex_ItemCode);
			QString strCurCode(cell->readValue().toString());
			cell = xlsx.cellAt(row, m_stTestItemExcelInfo.nColIndex_ItemName);
			QString strCurName(cell->readValue().toString());
			if (m_mapTestItemDictionary.find(strCurCode) == m_mapTestItemDictionary.end())
			{
				m_mapTestItemDictionary[strCurCode] = strCurName;
			}
			else
			{
				QMessageBox::critical(nullptr, "critical", QString::fromStdWString(L"%1”–÷ÿ∏¥").arg(strCurCode));
				return false;
			}
		}
	}
	return true;
}
