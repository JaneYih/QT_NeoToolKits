#pragma once

#include <QObject>
#include <QMap>
#include "DbScriptEditor_def.h"
#include "ExcelOperation.h"

class IniOperation;
class DbScriptDataModel;

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DbScriptOperate;
	class DbScriptEditorApp : public QObject
	{
		Q_OBJECT

	public:
		DbScriptEditorApp(QObject* parent = nullptr);
		~DbScriptEditorApp();

		void LoadExcelColumns(const QString& fileName);
		bool LoadExcelTestItemDictionary();
		QMap<QString, TestItem> getTestItemDictionary() const;
		TestItemExcelInfo getTestItemExcelInfo() const;
		void setTestItemExcelInfo(const TestItemExcelInfo& src);
		static QList<TestItem> TestItemsTextConverter(const QString& testItemsText,
			const QMap<QString, TestItem>& testItemDictionary);
		static bool ExportTestItems_SaveAsExcel(const QString& excelName,
			const QMap<QString, QList<TestItem>>& testItemListSheets, QString& strErrorMsg);

		QString getSQLiteDbPath() const;
		QString getDbScriptDefaultDirPath() const;
		bool OpenSQLiteDb(DbScriptDataModel* model, const QString& dbPath);
		void CloaseSQLiteDb(DbScriptDataModel* model = nullptr);
		bool SaveSQLiteData(DbScriptDataModel* model, QString& strErrorMsg);
		bool RefreshSQLiteData(DbScriptDataModel* model, int orderByFieldIndex, Qt::SortOrder order = Qt::DescendingOrder);

	private:
		void setSQLiteDbPath(const QString& src);
		void setDbScriptDefaultDirPath(const QString& src);
		static bool setExcelColumnContent(void* xlsx, void* format, const QString& strText,
			int rowIndex, int columnsIndex, int widthOffset, int& WrapCountMax);
		QString GetExcelCellValue(const QXlsx::Document& xlsx,
			const QList<QXlsx::CellRange>& qMergedCellsList,
			int row, int col);

	private:
		QString m_strIniFileName;
		IniOperation* m_pCfg;

		TestItemExcelInfo m_stTestItemExcelInfo;
		QMap<QString, TestItem> m_mapTestItemDictionary; //测试项代号与测试项名称关系表

		QString m_strSQLiteDbPath;
		QString m_strDbScriptDefaultDirPath;
		DbScriptOperate* m_DbScriptOperate;
	};
}


