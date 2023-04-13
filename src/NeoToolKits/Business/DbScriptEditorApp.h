#pragma once

#include <QObject>
#include <QMap>
#include "DbScriptEditor_def.h"

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
	
		QString getSQLiteDbPath() const;
		QString getDbScriptDefaultDirPath() const;
		bool OpenSQLiteDb(DbScriptDataModel* model, const QString& dbPath);
		void CloaseSQLiteDb(DbScriptDataModel* model = nullptr);
		bool SaveSQLiteData(DbScriptDataModel* model, QString& strErrorMsg);
		bool RefreshSQLiteData(DbScriptDataModel* model, int orderByFieldIndex, Qt::SortOrder order = Qt::DescendingOrder);

	private:
		void setSQLiteDbPath(const QString& src);
		void setDbScriptDefaultDirPath(const QString& src);

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


