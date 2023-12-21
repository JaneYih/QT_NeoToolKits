#include "DBScriptTestItemsEditorPopDialog.h"
#include <QPushButton>
#include <QCloseEvent>
#include <QMessageBox>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QDateTime>
#include "DBScriptTestItemsModel.h"
#include "DBScriptTestItemsDelegate.h"
#include "DbScriptEditorApp.h"
#include <QMessageBox>

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DBScriptTestItemsEditorPopDialog::DBScriptTestItemsEditorPopDialog(const QString& testItemsText,
	QMap<QString, TestItem>* hTestItemDictionary, QWidget* parent)
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
	connect(ui->btn_export, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_add, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_delete, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_refresh, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_apply, &QPushButton::clicked, this, &DBScriptTestItemsEditorPopDialog::PushbuttonClickedSlot);
	connect(ui->textEdit_TestItems, &QTextEdit::textChanged, this, &DBScriptTestItemsEditorPopDialog::TextEditTextChangedSlot);
	connect(m_testItemsModel, &DBScriptTestItemsModel::testItemsRowsMoved, this, &DBScriptTestItemsEditorPopDialog::TestItemsRowsMovedSlot);
	ui->tableView_TestItems->installEventFilter(this);
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
	//setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint);

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

bool DBScriptTestItemsEditorPopDialog::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui->tableView_TestItems)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			int curKey = keyEvent->key();
			if (keyEvent->modifiers() == Qt::ControlModifier
				&& (curKey == Qt::Key_C || curKey == Qt::Key_D))
			{
				QModelIndexList selectedIndexes = ui->tableView_TestItems->selectionModel()->selectedIndexes();
				QList<TestItem*> testitems;
				for each (QModelIndex var in selectedIndexes)
				{
					if (var.isValid() && var.column() == 1)
					{
						TestItem* testitem = static_cast<TestItem*>(var.internalPointer());
						if (testitem)
						{
							testitems.push_back(testitem);
						}
					}
				}

				QString strCopyText("");
				if (curKey == Qt::Key_C)
				{
					for each (auto var in testitems)
					{
						strCopyText += QString("%1=1\r\n").arg(var->name());
					}
				}
				else if (curKey == Qt::Key_D)
				{
					strCopyText += GetTemplateCodeString1(testitems);
					strCopyText += GetTemplateCodeString2(testitems);
					strCopyText += GetTemplateCodeString3(testitems);
					strCopyText += GetTemplateCodeString4(testitems);
					strCopyText += GetTemplateCodeString5(testitems);
					strCopyText += GetTemplateCodeString6(testitems);
					strCopyText += GetTemplateCodeString7(testitems);
				}

				QGuiApplication::clipboard()->setText(strCopyText);
				ui->tableView_TestItems->clearSelection();
				return true;
			}
		}
	}
	return QWidget::eventFilter(obj, event);
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString1(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	装载样式1\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("AddTestItemToDictionary(\"%1\", \"%2\", &CDlgTestUnit::TestUnit_%1);\r\n").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString2(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	装载样式2-名字装载\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("\
else if (\"%1\" == TestItemCode)\r\n\
{\r\n\
	return \"%2\";\r\n\
}\r\n\
").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString3(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	装载样式2-函数指针装载\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("\
else if (\"%1\" == TestItemCode)\r\n\
{\r\n\
	return &CDlgTestUnit::TestUnit_%1; //%2;\r\n\
}\r\n\
").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString4(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	CDlgTestUnit的声明\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("BOOL TestUnit_%1(const CString& testName, CString& errMsg);   //%2 \r\n").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString5(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	CDlgTestUnit的定义\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("\
//Test Unit: %2 \r\n\
BOOL CDlgTestUnit::TestUnit_%1(const CString& testName, CString& errMsg)\r\n\
{\r\n\
	if(m_cTestUnit->Test_%1(g_testDevMode, errMsg))\r\n\
	{\r\n\
		return TRUE;\r\n\
	}\r\n\
	return FALSE;\r\n\
}\r\n\r\n\
").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString6(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	CTestUnit的声明\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("BOOL Test_%1(testMode target, CString& errMsg);   //%2 \r\n").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

QString DBScriptTestItemsEditorPopDialog::GetTemplateCodeString7(const QList<TestItem*>& selectedTestitems)
{
	QString text = QString::fromStdWString(L"/***************************\r\n*	CTestUnit的定义\r\n***************************/\r\n");
	for each (auto var in selectedTestitems)
	{
		text += QString("\
//Test Unit: %2 \r\n\
BOOL CTestUnit::Test_%1(testMode target, CString& errMsg)\r\n\
{\r\n\
	return FALSE;\r\n\
}\r\n\r\n\
").arg(var->code()).arg(var->name());
	}
	return text + "\r\n\r\n\r\n";
}

void DBScriptTestItemsEditorPopDialog::ResetTestItemTableByText(const QString& testItemsText)
{
	QList<TestItem> testitems = DbScriptEditorApp::TestItemsTextConverter(testItemsText, *m_hTestItemDictionary);
	m_testItemsModel->resetTestItems(testitems);
}

void DBScriptTestItemsEditorPopDialog::TestItemsRowsMovedSlot()
{
	ui->tableView_TestItems->clearSelection();
	ui->tableView_TestItems->setCurrentIndex(QModelIndex());
}

void DBScriptTestItemsEditorPopDialog::TextEditTextChangedSlot()
{
	setAppliedFlag(true);
	refresh();
}

void DBScriptTestItemsEditorPopDialog::setTestmodeText(const QString& testmode)
{
	m_testmodeText = testmode;
}

QString DBScriptTestItemsEditorPopDialog::getTestmodeText() const
{
	return m_testmodeText;
}

void DBScriptTestItemsEditorPopDialog::setModelText(const QString& model)
{
	m_modelText = model;
}

QString DBScriptTestItemsEditorPopDialog::getModelText() const
{
	return m_modelText;
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
	else if (curBtn == ui->btn_export)
	{
		QString excelFileName = QFileDialog::getSaveFileName(this,
			QString::fromStdWString(L"保存文件"),
			QString::fromStdWString(L"%1/[%2]生产测试项清单_[%3]")
			.arg(QDir::homePath())
			.arg(getModelText())
			.arg(QDateTime::currentDateTime().toString("yyyyMMdd_hh_mm_ss")),
			"Excel File(*.xlsx)");
		if (!excelFileName.isEmpty())
		{
			ui->btn_export->setEnabled(false);
			QString strErrorMsg;
			if (DbScriptEditorApp::ExportTestItems_SaveAsExcel(excelFileName, QString("<%1><%2>").arg(getTestmodeText()).arg(getModelText()), m_testItemsModel->getTestItems(), strErrorMsg))
			{
				QMessageBox::information(this, "tip", QString::fromStdWString(L"导出成功！！"));
			}
			else
			{
				QMessageBox::warning(this, "error", QString::fromStdWString(L"导出失败：%1").arg(strErrorMsg));
			}
		}
		ui->btn_export->setEnabled(true);
	}
	else if (curBtn == ui->btn_add)
	{
		m_testItemsModel->insertRow(ui->tableView_TestItems->selectionModel()->currentIndex());
		ui->tableView_TestItems->clearSelection();
	}
	else if (curBtn == ui->btn_delete)
	{
		//m_testItemsModel->removeWaitingInsertRow(ui->tableView_TestItems->selectionModel()->currentIndex());
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
	if (testItemsText != ui->textEdit_TestItems->toPlainText())
	{
		ui->textEdit_TestItems->setText(testItemsText);
		setAppliedFlag(true);
	}
	else
	{
		refresh();
	}
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
