#pragma once

#include <QDialog>
#include <QMap>
#include "ui_DBScriptTestItemsEditorPopDialog.h"
#include "DbScriptEditor_def.h"

class DBScriptTestItemsModel;

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DBScriptTestItemsDelegate;
	class DBScriptTestItemsEditorPopDialog : public QDialog
	{
		Q_OBJECT

	public:
		explicit DBScriptTestItemsEditorPopDialog(const QString& testItemsText, 
			QMap<QString, QString>* hTestItemDictionary, QWidget* parent = nullptr);
		~DBScriptTestItemsEditorPopDialog();

		QString getTestItemsText() const {
			return m_testItemsText;
		}

	public slots:
		void PushbuttonClickedSlot(bool checked = false);

	protected:
		virtual void closeEvent(QCloseEvent* event) override;
		virtual void reject() override;

	private:
		void initView(void);
		void refresh();
		void apply();
		void ResetTestItemTableByText(const QString& testItemsText);

	private:
		Ui::DBScriptTestItemsEditorPopDlg* ui;
		QString m_testItemsText;
		QMap<QString, QString>* m_hTestItemDictionary;
		DBScriptTestItemsModel* m_testItemsModel;
		DBScriptTestItemsDelegate* m_testItemsDelegate;
	};
}

