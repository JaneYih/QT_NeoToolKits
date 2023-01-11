#include "DBScriptTestItemsEditorPopDialog.h"
#include <QPushButton>
#include <QCloseEvent>

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DBScriptTestItemsEditorPopDialog::DBScriptTestItemsEditorPopDialog(const QString& testItemsText, QWidget* parent)
	: QDialog(parent),
	ui(new Ui::DBScriptTestItemsEditorPopDlg),
	m_TestItemsText(testItemsText)
{
	initView();
	connect(ui->btn_ok, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_cancel, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
}

DBScriptTestItemsEditorPopDialog::~DBScriptTestItemsEditorPopDialog()
{
	delete ui;
}

void DBScriptTestItemsEditorPopDialog::initView()
{
	ui->setupUi(this);
	ui->textEdit_TestItems->setText(m_TestItemsText);
}

void DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_ok)
	{
		m_TestItemsText = ui->textEdit_TestItems->toPlainText();
		setResult(QDialog::Accepted);
		close();
	}
	else if (curBtn == ui->btn_cancel)
	{
		setResult(QDialog::Rejected);
		close();
	}
}

void DBScriptTestItemsEditorPopDialog::closeEvent(QCloseEvent* event)
{
	event->accept();
}

void DBScriptTestItemsEditorPopDialog::reject()
{
	close();
}
