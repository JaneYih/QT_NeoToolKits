#include "DbScriptEditorApp.h"
#include "Database_def.h"
#include "ExcelOperation.h"
#include "DataTableTest.h"
#include "IniOperation.h"
#include "DbScriptOperate.h"
#include "DbScriptDataModel.h"
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include <QSharedPointer>
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
		bool isEmptyRow = row->isEmptyRow();
		foreach(auto cell, row->fields)
		{
			if (cell.isWaitingInsert() && !isEmptyRow)
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

QString DbScriptEditorApp::GetExcelCellValue(const QXlsx::Document& xlsx,
	const QList<QXlsx::CellRange>& qMergedCellsList,
	int row, int col)
{
	using namespace QXlsx;
	QString value("");
	Cell* cell = nullptr;
	cell = xlsx.cellAt(row, col);
	foreach(QXlsx::CellRange cellR, qMergedCellsList)
	{
		//合并单元格可通过第一行，最后一行，第一列，最后一列四个参数来确定合并的区域
		int nFirstRow = cellR.firstRow();
		int nLastRow = cellR.lastRow();
		int nFirstCol = cellR.firstColumn();
		int nLastCol = cellR.lastColumn();
		//判断该单元格是否处于合并单元格中
		if (nFirstRow <= row && row <= nLastRow
			&& nFirstCol <= col && col <= nLastCol)
		{
			cell = xlsx.cellAt(nFirstRow, nFirstCol);
			break;
		}
	}
	if (cell != nullptr)
	{
		value = cell->value().toString();
	}

	return value;
}

bool DbScriptEditorApp::LoadExcelTestItemDictionary()
{
	using namespace QXlsx;
	m_mapTestItemDictionary.clear();

	if (!m_stTestItemExcelInfo.isValid())
	{
		return false;
	}

	QXlsx::Document xlsx(m_stTestItemExcelInfo.strExcelPath);
	CellRange cellRange = ExcelOperation::OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		int rowCount = cellRange.rowCount();
		int columnCount = cellRange.columnCount();

		int StartRowIndex = 2;
		for (int row = StartRowIndex; row < rowCount; ++row)
		{
			TestItem curTestItem;
			auto qMergedCellsList = xlsx.currentWorksheet()->mergedCells();

			QString strCurCode = GetExcelCellValue(xlsx, qMergedCellsList,
				row, m_stTestItemExcelInfo.nColIndex_ItemCode);
			curTestItem.setCode(strCurCode);

			QString strCurName = GetExcelCellValue(xlsx, qMergedCellsList,
				row, m_stTestItemExcelInfo.nColIndex_ItemName);
			curTestItem.setName(strCurName);

			QString strCurATComand = GetExcelCellValue(xlsx, qMergedCellsList,
				row, m_stTestItemExcelInfo.nColIndex_ItemATCommand);
			curTestItem.setATComand(strCurATComand);

			QString strCurRemark = GetExcelCellValue(xlsx, qMergedCellsList,
				row, m_stTestItemExcelInfo.nColIndex_ItemRemark);
			curTestItem.setRemark(strCurRemark);
			
			if (m_mapTestItemDictionary.find(strCurCode) == m_mapTestItemDictionary.end())
			{
				m_mapTestItemDictionary[strCurCode] = curTestItem;
			}
			else
			{
				QMessageBox::critical(nullptr, "critical", QString::fromStdWString(L"%1有重复").arg(strCurCode));
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
		invalidItem.setName(QString::fromStdWString(L"无效符号"));
		invalidItem.setCode(var);
		invalidItem.setWaitingDelete();
		testitems.push_back(invalidItem);
	}
	return testitems;
}

bool DbScriptEditorApp::ExportTestItems_SaveAsExcel(const QString& excelName,
	const QMap<QString, QList<TestItem>>& testItemListSheets, QString& strErrorMsg)
{
	if (testItemListSheets.size() <= 0)
	{
		strErrorMsg = QString::fromStdWString(L"请选择%1列！").arg(DbScriptDataModel::s_TestListHeaderName);
		return false;
	}

	using namespace QXlsx;
	QFileInfo fileInfo(excelName);
	QSharedPointer<QXlsx::Document> xlsx = nullptr;
	if (fileInfo.exists())
	{
		xlsx = QSharedPointer<QXlsx::Document>(new QXlsx::Document(excelName));
	}
	else
	{
		xlsx = QSharedPointer<QXlsx::Document>(new QXlsx::Document());
	}

	QMapIterator<QString, QList<TestItem>> i(testItemListSheets);
	while (i.hasNext())
	{
		i.next();
		const QList<TestItem> testitemList = i.value();
		const QString sheetName = i.key();

		auto sheetNames = xlsx->sheetNames();
		if (sheetNames.contains(sheetName))
		{
			xlsx->deleteSheet(sheetName);
		}
		xlsx->addSheet(sheetName);
		xlsx->selectSheet(sheetName);

		QXlsx::Format format1;
		format1.setFontBold(true);
		format1.setFontColor(QColor(Qt::black));
		format1.setBorderStyle(Format::BorderThin);
		format1.setFontSize(12);
		format1.setVerticalAlignment(Format::AlignVCenter);
		format1.setHorizontalAlignment(Format::AlignLeft);
		format1.setTextWarp(true); //自动换行

		if (testitemList.size() <= 0)
		{
			strErrorMsg = QString::fromStdWString(L"测试项列表为空，无法导出数据！");
			xlsx->write(1, 1, strErrorMsg, format1);
			continue;
		}

		//标题
		int ColumnsIndex = 1;
		int rowIndex = 1;
		int widthOffset = 10;
		QString strText = QString::fromStdWString(L"代号");
		xlsx->setColumnWidth(ColumnsIndex, strText.length() + widthOffset);
		xlsx->write(rowIndex, ColumnsIndex++, strText, format1);

		strText = QString::fromStdWString(L"测试项名称");
		xlsx->setColumnWidth(ColumnsIndex, strText.length() + widthOffset);
		xlsx->write(rowIndex, ColumnsIndex++, strText, format1);

		strText = QString::fromStdWString(L"测试指令");
		xlsx->setColumnWidth(ColumnsIndex, strText.length() + widthOffset);
		xlsx->write(rowIndex, ColumnsIndex++, strText, format1);

		strText = QString::fromStdWString(L"备注");
		xlsx->setColumnWidth(ColumnsIndex, strText.length() + widthOffset);
		xlsx->write(rowIndex, ColumnsIndex++, strText, format1);

		xlsx->setRowHeight(rowIndex, 25);

		//内容
		format1.setFontBold(false);
		format1.setFontColor(QColor(Qt::black));
		rowIndex = 2;
		int iDataIndex = 0;
		for each (auto testItem in testitemList)
		{
			ColumnsIndex = 1;
			int WrapCountMax = 1; //内容换行符个数最大值
			setExcelColumnContent(xlsx.get(), &format1, testItem.code(), rowIndex, ColumnsIndex++, widthOffset, WrapCountMax);
			setExcelColumnContent(xlsx.get(), &format1, testItem.name(), rowIndex, ColumnsIndex++, widthOffset, WrapCountMax);
			setExcelColumnContent(xlsx.get(), &format1, testItem.atComand(), rowIndex, ColumnsIndex++, widthOffset, WrapCountMax);
			setExcelColumnContent(xlsx.get(), &format1, testItem.remark(), rowIndex, ColumnsIndex++, widthOffset, WrapCountMax);
			xlsx->setRowHeight(rowIndex++, 25 * WrapCountMax);
		}
	}

	QString fileFullName = QString("%1/%2.%4")
		.arg(fileInfo.absoluteDir().absolutePath())
		.arg(fileInfo.baseName())
		.arg(fileInfo.suffix());
	if (!xlsx->saveAs(fileFullName))
	{
		strErrorMsg = QString::fromStdWString(L"excel文件保存失败：%1").arg(fileFullName);
		return false;
	}

	//自动调用缺省的电脑应用软件打开文件
	bool bOpenUrlSucceed = QDesktopServices::openUrl(QUrl(fileFullName, QUrl::TolerantMode));

	return true;
}

bool DbScriptEditorApp::setExcelColumnContent(void* xlsx, void* format,
	const QString& strText, int rowIndex, int columnsIndex, int widthOffset, int& WrapCountMax)
{
	QXlsx::Document* pxlsx = static_cast<QXlsx::Document*>(xlsx);
	QXlsx::Format* format1 = static_cast<QXlsx::Format*>(format);

	if (pxlsx != nullptr)
	{
		if (!strText.isEmpty())
		{
			int newColumnWidth = strText.length() + widthOffset;
			int oldColumnWidth = pxlsx->columnWidth(columnsIndex);
			if (newColumnWidth > oldColumnWidth)
			{
				pxlsx->setColumnWidth(columnsIndex, newColumnWidth);
			}
		}

		int WrapCount = strText.count('\n') + 1;
		WrapCountMax = WrapCount > WrapCountMax ? WrapCount : WrapCountMax;
		return pxlsx->write(rowIndex, columnsIndex, strText, *format1);
	}

	return false;
}