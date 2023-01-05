#pragma once

#include <QObject>
#include <QMap>
#include "DbScriptEditor_def.h"

class IniOperation;

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DbScriptEditorApp : public QObject
	{
		Q_OBJECT

	public:
		DbScriptEditorApp(QObject* parent = nullptr);
		~DbScriptEditorApp();

		void LoadExcelColumns(const QString& fileName);
		bool LoadExcelTestItemDictionary();
		QMap<QString, QString> getTestItemDictionary() const;
		TestItemExcelInfo getTestItemExcelInfo() const;
		void setTestItemExcelInfo(const TestItemExcelInfo& src);
	
		QString getSQLiteDbPath() const;
		void setSQLiteDbPath(const QString& src);
		bool TestSqliteDb(const QString& dbPath, const QString& tableName);

	private:
		QString m_strIniFileName;
		IniOperation* m_pCfg;

		TestItemExcelInfo m_stTestItemExcelInfo;
		QMap<QString, QString> m_mapTestItemDictionary; //测试项代号与测试项名称关系表

		QString m_strSQLiteDbPath;
	};
}


