#pragma once

#include <QDialog>
#include "ui_DBScriptTestItemsEditorPopDialog.h"
#include "DbScriptEditor_def.h"

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DBScriptTestItemsEditorPopDialog : public QDialog
	{
		Q_OBJECT

	public:
		DBScriptTestItemsEditorPopDialog(const QString& testItemsText, QWidget* parent = nullptr);
		~DBScriptTestItemsEditorPopDialog();

		QString getTestItemsText() const {
			return m_TestItemsText;
		}

	public slots:
		void PushbuttonClickedSlot(bool checked = false);

	protected:
		virtual void closeEvent(QCloseEvent* event) override;
		virtual void reject() override;

	private:
		void initView(void);

	private:
		Ui::DBScriptTestItemsEditorPopDlg* ui;
		QString m_TestItemsText;
	};
}

