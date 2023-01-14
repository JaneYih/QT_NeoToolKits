#pragma once

#include <QWidget>
#include "ui_DbScriptEditorPageForm.h"
#include "DbScriptEditor_def.h"

class DbScriptDataModel;

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DbScriptEditorApp;
	class DbScriptEditorPageForm : public QWidget
	{
		Q_OBJECT

	public:
		DbScriptEditorPageForm(QWidget* parent = nullptr);
		~DbScriptEditorPageForm();

	protected:
		virtual void showEvent(QShowEvent* event) override;
		virtual bool eventFilter(QObject* obj, QEvent* event) override;

	public slots:
		void PushbuttonClickedSlot(bool checked = false);
		void ComboBoxCurrentIndexChangedSlot(int index);
		void HorizontalHeaderSectionClickedSlot(int logicalIndex);
		void DBDataTableItemDoubleClickedSlot(const QModelIndex& index);

	private:
		void initView(void);
		void InitComboBoxItems(const TestItemExcelInfo& info);
		void LoadExcelInfo(const QString& fileName);
		void LoadSQLiteDb(const QString& dbPath);
		void Refresh();
		void LoadItemDictionary();

	private:
		Ui::DbScriptEditorPageForm* ui;
		DbScriptEditorApp* m_pApp;
		DbScriptDataModel* m_pDataModel;
		volatile bool m_bFirstShowData;
	};
}

