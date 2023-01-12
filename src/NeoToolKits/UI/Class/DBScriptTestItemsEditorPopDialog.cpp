#include "DBScriptTestItemsEditorPopDialog.h"
#include <QPushButton>
#include <QCloseEvent>
#include "DBScriptTestItemsModel.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DBScriptTestItemsEditorPopDialog::DBScriptTestItemsEditorPopDialog(const QString& testItemsText, QWidget* parent)
	: QDialog(parent),
	ui(new Ui::DBScriptTestItemsEditorPopDlg),
	m_testItemsText(testItemsText),
	m_testItemsModel(new DBScriptTestItemsModel(this))
{
	Q_ASSERT(m_testItemsModel);

	initView();
	connect(ui->btn_ok, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_cancel, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_add, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_delete, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_apply, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
}

DBScriptTestItemsEditorPopDialog::~DBScriptTestItemsEditorPopDialog()
{
	delete ui;
}

void DBScriptTestItemsEditorPopDialog::initView()
{
	ui->setupUi(this);
	ui->textEdit_TestItems->setText(m_testItemsText);

	ui->tableView_TestItems->setModel(m_testItemsModel);
	QHeaderView* horizontalHeader = ui->tableView_TestItems->horizontalHeader();
	horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader->setStretchLastSection(true);
	ui->tableView_TestItems->setAlternatingRowColors(true);
	ui->tableView_TestItems->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui->tableView_TestItems->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui->tableView_TestItems->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	ui->tableView_TestItems->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	ui->tableView_TestItems->show();
}

void DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_ok)
	{
		m_testItemsText = ui->textEdit_TestItems->toPlainText();
		setResult(QDialog::Accepted);
		close();
	}
	else if (curBtn == ui->btn_cancel)
	{
		setResult(QDialog::Rejected);
		close();
	}
	else if (curBtn == ui->btn_add)
	{
		m_testItemsModel->insertRow(ui->tableView_TestItems->selectionModel()->currentIndex());
		ui->tableView_TestItems->clearSelection();
	}
	else if (curBtn == ui->btn_delete)
	{
		m_testItemsModel->removeRows(ui->tableView_TestItems->selectionModel()->selectedIndexes());
		ui->tableView_TestItems->clearSelection();
	}
	else if (curBtn == ui->btn_apply)
	{
		/*QList<TestItem> items;
		items.push_back(TestItem("111", "111"));
		items.push_back(TestItem("222", "222"));
		items.push_back(TestItem("333", "333"));
		items.push_back(TestItem("444", "444"));
		m_testItemsModel->resetTestItems(items);*/

		/*TestItem strValue;
		strValue.setCode("test");
		strValue.setName("name");
		m_testItemsModel->setTestItem(ui->tableView_TestItems->selectionModel()->currentIndex(), strValue);
		ui->tableView_TestItems->clearSelection();*/
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
