#include "DBScriptTestItemsEditorPopDialog.h"
#include <QPushButton>
#include <QCloseEvent>
#include "DBScriptTestItemsModel.h"
#include "DBScriptTestItemsDelegate.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DBScriptTestItemsEditorPopDialog::DBScriptTestItemsEditorPopDialog(const QString& testItemsText,
	QMap<QString, QString>* hTestItemDictionary, QWidget* parent)
	: QDialog(parent),
	ui(new Ui::DBScriptTestItemsEditorPopDlg),
	m_testItemsText(testItemsText),
	m_hTestItemDictionary(hTestItemDictionary),
	m_testItemsModel(new DBScriptTestItemsModel(this)),
	m_testItemsDelegate(new DBScriptTestItemsDelegate(hTestItemDictionary, this))
{
	Q_ASSERT(m_hTestItemDictionary);
	Q_ASSERT(m_testItemsModel);
	Q_ASSERT(m_testItemsDelegate);
	initView();
	connect(ui->btn_ok, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_cancel, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_add, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_delete, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_refresh, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_apply, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
}

DBScriptTestItemsEditorPopDialog::~DBScriptTestItemsEditorPopDialog()
{
	delete m_testItemsDelegate;
	delete m_testItemsModel;
	delete ui;
}

void DBScriptTestItemsEditorPopDialog::initView()
{
	ui->setupUi(this);
	ui->textEdit_TestItems->setText(m_testItemsText);

	ui->tableView_TestItems->setModel(m_testItemsModel);
	ui->tableView_TestItems->setItemDelegateForColumn(1, m_testItemsDelegate);
	QHeaderView* horizontalHeader = ui->tableView_TestItems->horizontalHeader();
	horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader->setStretchLastSection(true);
	ui->tableView_TestItems->verticalHeader()->hide();
	ui->tableView_TestItems->setAlternatingRowColors(true);
	ui->tableView_TestItems->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui->tableView_TestItems->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui->tableView_TestItems->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	ui->tableView_TestItems->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
	ui->tableView_TestItems->show();

	refresh();
}

void DBScriptTestItemsEditorPopDialog::ResetTestItemTableByText(const QString& testItemsText)
{
	QStringList testCodeItems = testItemsText.split(';', QString::SkipEmptyParts);
	QList<TestItem> testitems;
	for each (auto var in testCodeItems)
	{
		if (m_hTestItemDictionary->find(var) != m_hTestItemDictionary->end())
		{
			TestItem item(var, (*m_hTestItemDictionary)[var]);
			if (item.isValid())
			{
				testitems.push_back(item);
			}
		}
	}
	m_testItemsModel->resetTestItems(testitems);
}

void DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_ok)
	{
		apply();
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
	else if (curBtn == ui->btn_refresh)
	{
		refresh();
	}
	else if (curBtn == ui->btn_apply)
	{
		apply();
	}
}

void DBScriptTestItemsEditorPopDialog::refresh()
{
	QString testItemsText(ui->textEdit_TestItems->toPlainText());
	ResetTestItemTableByText(testItemsText);
	ui->tableView_TestItems->clearSelection();
}

void DBScriptTestItemsEditorPopDialog::apply()
{
	m_testItemsModel->removeWaitingDeleteRows();

	QString testItemsText;
	for each (auto var in m_testItemsModel->getTestItems())
	{
		if (var.isValid())
		{
			testItemsText += QString("%1;").arg(var.code());
		}
	}
	ui->textEdit_TestItems->setText(testItemsText);
	refresh();
}

void DBScriptTestItemsEditorPopDialog::closeEvent(QCloseEvent* event)
{
	event->accept();
}

void DBScriptTestItemsEditorPopDialog::reject()
{
	close();
}
