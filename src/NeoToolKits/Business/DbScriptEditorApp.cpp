#include "DbScriptEditorApp.h"
#include "Database_def.h"
#include "DataTableTest.h"
#include "ExcelOperation.h"
#include "IniOperation.h"
#include "DbScriptOperate.h"
#include "DbScriptDataModel.h"
#include <QMessageBox>
#include <QDir>
using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

//ini prefixs
const QString s_ini_prefix_excel = "Excel";
const QString s_ini_prefix_DbScript = "DbScript";

//ini keys
const QString s_ini_key_excelFile = "ExcelFlie";
const QString s_ini_key_ColumnIndex_TestItemCode = "ColumnIndex_TestItemCode";
const QString s_ini_key_ColumnIndex_TestItemName = "ColumnIndex_TestItemName";
const QString s_ini_key_DbScriptDefaultDirPath = "DbScriptDefaultDirPath";

DbScriptEditorApp::DbScriptEditorApp(QObject *parent)
	: QObject(parent),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/DbScriptEditorTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName)),
	m_DbScriptOperate(nullptr)
{
	m_stTestItemExcelInfo.strExcelPath = m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString();
	m_stTestItemExcelInfo.nColIndex_ItemCode = m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_ColumnIndex_TestItemCode, -1).toInt();
	m_stTestItemExcelInfo.nColIndex_ItemName = m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_ColumnIndex_TestItemName, -1).toInt();
	m_stTestItemExcelInfo.nColIndex_ItemATCommand = m_stTestItemExcelInfo.nColIndex_ItemName + 1;
	m_stTestItemExcelInfo.nColIndex_ItemRemark = m_stTestItemExcelInfo.nColIndex_ItemName + 2;
	m_strDbScriptDefaultDirPath = m_pCfg->ReadValue(s_ini_prefix_DbScript, s_ini_key_DbScriptDefaultDirPath, QDir::homePath()).toString();
}

DbScriptEditorApp::~DbScriptEditorApp()
{
	CloaseSQLiteDb();
	delete m_pCfg;
}

QMap<QString, TestItem> DbScriptEditorApp::getTestItemDictionary() const
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

QString DbScriptEditorApp::getDbScriptDefaultDirPath() const
{
	return m_strDbScriptDefaultDirPath;
}

void DbScriptEditorApp::setDbScriptDefaultDirPath(const QString& src)
{
	m_strDbScriptDefaultDirPath = src;
	m_pCfg->WriteValue(s_ini_prefix_DbScript, s_ini_key_DbScriptDefaultDirPath, src);
}

void DbScriptEditorApp::CloaseSQLiteDb(DbScriptDataModel* model)
{
	if (m_DbScriptOperate)
	{
		delete m_DbScriptOperate;
		m_DbScriptOperate = nullptr;
	}
	if (model)
	{
		model->ClearDbScriptData();
	}
}

bool DbScriptEditorApp::OpenSQLiteDb(DbScriptDataModel* model, const QString& dbPath)
{
	if (model == nullptr)
	{
		return false;
	}

	CloaseSQLiteDb(model);

	SqlTableInfo tempInfo;
	tempInfo.baseInfo.type = SqlTypes::eSQLITE;
	tempInfo.baseInfo.dbName = dbPath;
	m_DbScriptOperate = new DbScriptOperate(tempInfo);
	if (m_DbScriptOperate)
	{	
		if (m_DbScriptOperate->TestConnect())
		{
			setSQLiteDbPath(dbPath);
			QFileInfo fileInfo(dbPath);
			setDbScriptDefaultDirPath(fileInfo.path());

			DbData outData;
			QString strErrorMsg;
			if (!m_DbScriptOperate->GetTableFullData(outData, strErrorMsg))
			{
				QMessageBox::critical(nullptr, "critical", strErrorMsg);
				return false;
			}
			model->setDbScriptData(outData);
			return true;
		}
	}
	return false;
}

bool DbScriptEditorApp::RefreshSQLiteData(DbScriptDataModel* model, int orderByFieldIndex, Qt::SortOrder order)
{
	if (m_DbScriptOperate == nullptr
		|| model == nullptr)
	{
		return false;
	}

	if (model)
	{
		DbData modelDbScriptData = model->getDbScriptData();
		QString orderByField("");
		if (orderByFieldIndex < modelDbScriptData.fieldGroup.fields.count())
		{
			orderByField = modelDbScriptData.fieldGroup.fields[orderByFieldIndex].value();
			DbData outData;
			QString strErrorMsg;
			if (!m_DbScriptOperate->GetTableFullData(outData, strErrorMsg, orderByField, order))
			{
				QMessageBox::critical(nullptr, "critical", strErrorMsg);
				return false;
			}
			model->setDbScriptData(outData);
		}
		return true;
	}
	return false;
}

bool DbScriptEditorApp::SaveSQLiteData(DbScriptDataModel* model, QString& strErrorMsg)
{
	if (m_DbScriptOperate == nullptr
		|| model == nullptr)
	{
		return false;
	}

	DbData SQLiteData = model->getDbScriptData();
	DbData WaitingDeleteData;
	DbData WaitingInsertData;
	DbData WaitingUpdateData;

	foreach (auto row, SQLiteData.rows)
	{
		foreach (auto cell, row->fields)
		{
			if (cell.isWaitingInsert())
			{
				WaitingInsertData.rows.push_back(new DbFieldGroup(*row));
				break;
			}
			else if (cell.isWaitingDelete())
			{
				WaitingDeleteData.rows.push_back(new DbFieldGroup(*row));
				break;
			}
			else if (cell.isWaitingUpdate())
			{
				WaitingUpdateData.rows.push_back(new DbFieldGroup(*row));
				break;
			}
		}
	}

	if (!WaitingInsertData.rows.isEmpty())
	{
		WaitingInsertData.fieldGroup = SQLiteData.fieldGroup;
		if (!m_DbScriptOperate->InsertData(WaitingInsertData, strErrorMsg))
		{
			return false;
		}
	}
	if (!WaitingUpdateData.rows.isEmpty())
	{
		WaitingUpdateData.fieldGroup = SQLiteData.fieldGroup;
		if (!m_DbScriptOperate->UpdateData(WaitingUpdateData, strErrorMsg))
		{
			return false;
		}
	}
	if (!WaitingDeleteData.rows.isEmpty())
	{
		WaitingDeleteData.fieldGroup = SQLiteData.fieldGroup;
		if (!m_DbScriptOperate->DeleteData(WaitingDeleteData, strErrorMsg))
		{
			return false;
		}
	}
	return true;
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
			TestItem curTestItem;

			Cell* cell = xlsx.cellAt(row, m_stTestItemExcelInfo.nColIndex_ItemCode);
			QString strCurCode(cell->readValue().toString());
			curTestItem.setCode(strCurCode);

			cell = xlsx.cellAt(row, m_stTestItemExcelInfo.nColIndex_ItemName);
			QString strCurName(cell->readValue().toString());
			curTestItem.setName(strCurName);

			cell = xlsx.cellAt(row, m_stTestItemExcelInfo.nColIndex_ItemATCommand);
			curTestItem.setATComand(cell->readValue().toString());

			cell = xlsx.cellAt(row, m_stTestItemExcelInfo.nColIndex_ItemRemark);
			curTestItem.setRemark(cell->readValue().toString());

			if (m_mapTestItemDictionary.find(strCurCode) == m_mapTestItemDictionary.end())
			{
				m_mapTestItemDictionary[strCurCode] = curTestItem;
			}
			else
			{
				QMessageBox::critical(nullptr, "critical", QString::fromStdWString(L"%1���ظ�").arg(strCurCode));
				return false;
			}
		}
	}
	return true;
}

QList<TestItem> DbScriptEditorApp::TestItemsTextConverter(const QString& testItemsText, 
	const QMap<QString, TestItem>& testItemDictionary)
{
	QStringList testCodeItems = testItemsText.split(';', QString::SkipEmptyParts);
	QList<TestItem> testitems;
	for each (auto var in testCodeItems)
	{
		if (testItemDictionary.find(var) != testItemDictionary.end())
		{
			TestItem item(testItemDictionary[var]);
			if (item.isValid())
			{
				testitems.push_back(item);
				continue;
			}
		}

		TestItem invalidItem;
		invalidItem.setName(QString::fromStdWString(L"��Ч����"));
		invalidItem.setCode(var);
		invalidItem.setWaitingDelete();
		testitems.push_back(invalidItem);
	}
	return testitems;
}