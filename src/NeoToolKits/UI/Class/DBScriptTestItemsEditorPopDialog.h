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
			QMap<QString, TestItem>* hTestItemDictionary, QWidget* parent = nullptr);
		~DBScriptTestItemsEditorPopDialog();

		QString getTestItemsText() const {
			return m_testItemsText;
		}

	public slots:
		void PushbuttonClickedSlot(bool checked = false);
		void TextEditTextChangedSlot();
		void TestItemsRowsMovedSlot();

	protected:
		virtual void closeEvent(QCloseEvent* event) override;
		virtual void reject() override;
		virtual bool eventFilter(QObject* obj, QEvent* event) override;

	private:
		void initView(void);
		void refresh();
		void apply();
		void ResetTestItemTableByText(const QString& testItemsText);
		void setTestItemsText(const QString& text);
		void setAppliedFlag(bool isApplied);
		bool getAppliedFlag() const;

		QString GetTemplateCodeString1(const QList<TestItem*>& selectedTestitems);
		QString GetTemplateCodeString2(const QList<TestItem*>& selectedTestitems);
		QString GetTemplateCodeString3(const QList<TestItem*>& selectedTestitems);
		QString GetTemplateCodeString4(const QList<TestItem*>& selectedTestitems);
		QString GetTemplateCodeString5(const QList<TestItem*>& selectedTestitems);
		QString GetTemplateCodeString6(const QList<TestItem*>& selectedTestitems);
		QString GetTemplateCodeString7(const QList<TestItem*>& selectedTestitems);

	private:
		Ui::DBScriptTestItemsEditorPopDlg* ui;
		QString m_testItemsText;
		QMap<QString, TestItem>* m_hTestItemDictionary;
		DBScriptTestItemsModel* m_testItemsModel;
		DBScriptTestItemsDelegate* m_testItemsDelegate;
		volatile bool m_bApplied;
	};
}

