#include "DBScriptTestItemsEditorPopDialog.h"
#include <QPushButton>
#include <QCloseEvent>
#include <QMessageBox>
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
	m_testItemsDelegate(new DBScriptTestItemsDelegate(hTestItemDictionary, this)),
	m_bApplied(false)
{
	Q_ASSERT(m_hTestItemDictionary);
	Q_ASSERT(m_testItemsModel);
	Q_ASSERT(m_testItemsDelegate);
	ui->setupUi(this);	
	initView();

	connect(ui->btn_ok, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_cancel, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_add, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_delete, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_refresh, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_apply, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->textEdit_TestItems, &QTextEdit::textChanged, this, &DBScriptTestItemsEditorPopDialog::TextEditTextChangedSlot);
}

DBScriptTestItemsEditorPopDialog::~DBScriptTestItemsEditorPopDialog()
{
	delete m_testItemsDelegate;
	delete m_testItemsModel;
	delete ui;
}

void DBScriptTestItemsEditorPopDialog::initView()
{
	setWindowFlags(Qt::Window);

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
	ui->tableView_TestItems->setDragEnabled(true);
	//ui->tableView_TestItems->setAcceptDrops(true);
	ui->tableView_TestItems->setDragDropMode(QAbstractItemView::InternalMove);
	ui->tableView_TestItems->setDropIndicatorShown(true);
	ui->tableView_TestItems->show();

	ui->textEdit_TestItems->setText(m_testItemsText);
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

void DBScriptTestItemsEditorPopDialog::TextEditTextChangedSlot()
{
	setAppliedFlag(true);
	refresh();
}

void DBScriptTestItemsEditorPopDialog::setTestItemsText(const QString& text)
{
	m_testItemsText = text;
	setAppliedFlag(false);
}

void DBScriptTestItemsEditorPopDialog::setAppliedFlag(bool isApplied)
{
	m_bApplied = isApplied;

	//ui->textEdit_TestItems->setTextColor(QColor(Qt::red));
	//ui->textEdit_TestItems->setTextBackgroundColor(QColor(isApplied ? Qt::yellow : Qt::white));

	QPalette plet = ui->textEdit_TestItems->palette();
	plet.setColor(QPalette::Text, QColor(isApplied ? Qt::red : Qt::black));
	ui->textEdit_TestItems->setPalette(plet);

	QString strRGB = isApplied ? "243, 245, 152" : "255, 255, 255";
	ui->textEdit_TestItems->setStyleSheet(QString("background-color: rgb(%1);").arg(strRGB));
}

bool DBScriptTestItemsEditorPopDialog::getAppliedFlag() const
{
	return m_bApplied;
}

void DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_ok)
	{
		apply();
		setTestItemsText(ui->textEdit_TestItems->toPlainText());
		setResult(QDialog::Accepted);
		close();
	}
	else if (curBtn == ui->btn_cancel)
	{
		setAppliedFlag(false);
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
	m_testItemsModel->removeWaitingOperateRows(TestItem::TestItemOperate::TestItem_Delete);

	QString testItemsText;
	for each (auto var in m_testItemsModel->getTestItems())
	{
		if (var.isValid())
		{
			testItemsText += QString("%1;").arg(var.code());
		}
	}
	ui->textEdit_TestItems->setText(testItemsText);
	setAppliedFlag(true);
}

void DBScriptTestItemsEditorPopDialog::closeEvent(QCloseEvent* event)
{
	if (getAppliedFlag())
	{
		if (QMessageBox::Cancel == QMessageBox::warning(this, "warning", QString::fromStdWString(L"数据还没保存，确定要退出吗?"), QMessageBox::Yes, QMessageBox::Cancel))
		{
			event->ignore();
			return;
		}
	}
	event->accept();
}

void DBScriptTestItemsEditorPopDialog::reject()
{
	close();
}
