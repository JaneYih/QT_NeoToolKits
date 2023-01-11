#include "DbScriptEditorPageForm.h"
#include <QDebug>
#include "DbScriptEditorApp.h"
#include "DbScriptDataModel.h"
#include <QFileDialog>
#include <QComboBox>
#include <QMessageBox>
#include "DBScriptTestItemsEditorPopDialog.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DbScriptEditorPageForm::DbScriptEditorPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::DbScriptEditorPageForm),
	m_pApp(new DbScriptEditorApp(this)),
	m_bFirstShowData(true),
	m_pDataModel(new DbScriptDataModel(this))
{
	Q_ASSERT(m_pApp);
	Q_ASSERT(m_pDataModel);
	initView();
	connect(ui->btn_ExcelPath, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_DBPath, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_DisconnectDB, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_LoadItemDictionary, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_add, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_delete, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_refresh, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_save, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->comboBox_testCodeCol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DbScriptEditorPageForm::ComboBoxCurrentIndexChangedSlot);
	connect(ui->comboBox_testNameCol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DbScriptEditorPageForm::ComboBoxCurrentIndexChangedSlot);
}

DbScriptEditorPageForm::~DbScriptEditorPageForm()
{
	delete m_pDataModel;
	delete m_pApp;
	delete ui;
}

void DbScriptEditorPageForm::initView(void)
{
	ui->setupUi(this);
}

void DbScriptEditorPageForm::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
	if (m_bFirstShowData)
	{
		LoadExcelInfo(m_pApp->getTestItemExcelInfo().strExcelPath);

		ui->tableView_DBDataTable->setModel(m_pDataModel);
		QHeaderView* horizontalHeader = ui->tableView_DBDataTable->horizontalHeader();
		horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
		horizontalHeader->setDefaultAlignment(Qt::AlignLeft);
		horizontalHeader->setStretchLastSection(true);
		horizontalHeader->setSortIndicatorShown(true);
		horizontalHeader->setSectionsClickable(true);
		connect(horizontalHeader, &QHeaderView::sectionClicked, this, &DbScriptEditorPageForm::HorizontalHeaderSectionClickedSlot);
		connect(ui->tableView_DBDataTable, &QAbstractItemView::doubleClicked, this, &DbScriptEditorPageForm::DBDataTableItemDoubleClickedSlot);
		ui->tableView_DBDataTable->setAlternatingRowColors(true);
		ui->tableView_DBDataTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
		ui->tableView_DBDataTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
		ui->tableView_DBDataTable->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
		ui->tableView_DBDataTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
		ui->tableView_DBDataTable->show();

		m_bFirstShowData = false;
	}
}

void DbScriptEditorPageForm::DBDataTableItemDoubleClickedSlot(const QModelIndex& index)
{
	if (index.isValid())
	{
		QString horizontalHeaderName = m_pDataModel->GetHorizontalHeaderName(index.column());
		if ("TESTLIST" == horizontalHeaderName)
		{
			DBScriptTestItemsEditorPopDialog dlg(m_pDataModel->getItemData(index), this);
			if (dlg.exec() == QDialog::Accepted)
			{
				m_pDataModel->setItemData(index, dlg.getTestItemsText());
			}
			ui->tableView_DBDataTable->clearSelection();
		}
	}
}

void DbScriptEditorPageForm::HorizontalHeaderSectionClickedSlot(int logicalIndex)
{
	static Qt::SortOrder order = Qt::AscendingOrder;
	if (Qt::DescendingOrder == order)
	{
		order = Qt::AscendingOrder;
	}
	else if (Qt::AscendingOrder == order)
	{
		order = Qt::DescendingOrder;
	}
	m_pApp->RefreshSQLiteData(m_pDataModel, logicalIndex, order);
	ui->tableView_DBDataTable->sortByColumn(logicalIndex, order);
	ui->tableView_DBDataTable->clearSelection();
}

void DbScriptEditorPageForm::InitComboBoxItems(const TestItemExcelInfo& info)
{
	ui->comboBox_testCodeCol->clear();
	ui->comboBox_testCodeCol->addItems(info.listExcelColumns);
	int curIndex = ui->comboBox_testCodeCol->count() >= info.nColIndex_ItemCode
		? info.nColIndex_ItemCode - 1
		: -1;
	ui->comboBox_testCodeCol->setCurrentIndex(curIndex);
	emit ui->comboBox_testCodeCol->currentIndexChanged(curIndex);

	ui->comboBox_testNameCol->clear();
	ui->comboBox_testNameCol->addItems(info.listExcelColumns);
	curIndex = ui->comboBox_testNameCol->count() >= info.nColIndex_ItemName
					? info.nColIndex_ItemName - 1
					: -1;
	ui->comboBox_testNameCol->setCurrentIndex(curIndex);
	emit ui->comboBox_testNameCol->currentIndexChanged(curIndex);
}

void DbScriptEditorPageForm::LoadExcelInfo(const QString& fileName)
{
	if (!fileName.isEmpty())
	{
		ui->lineEdit_ExcelPath->setText(fileName);
		m_pApp->LoadExcelColumns(fileName);
		InitComboBoxItems(m_pApp->getTestItemExcelInfo());
		if (m_pApp->getTestItemExcelInfo().listExcelColumns.size() >= 2)
		{
			ui->lineEdit_ExcelPath->setStyleSheet("background-color: rgb(0, 255, 0);");
			return;
		}
	}
	ui->comboBox_ItemDictionary->clear();
	ui->lineEdit_ExcelPath->setStyleSheet("background-color: rgb(255, 0, 0);");
}

void DbScriptEditorPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_ExcelPath)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Excel File"), "", tr("Excel Files (*.xlsx)"));
		LoadExcelInfo(fileName);
	}
	else if (curBtn == ui->btn_LoadItemDictionary)
	{
		m_pApp->LoadExcelTestItemDictionary();
		QMap<QString, QString> map = m_pApp->getTestItemDictionary();
		ui->comboBox_ItemDictionary->clear();
		QMapIterator<QString, QString> i(map);
		while (i.hasNext()) 
		{
			i.next();
			ui->comboBox_ItemDictionary->addItem(QString("%1:%2").arg(i.key()).arg(i.value()));
		}
	}
	else if (curBtn == ui->btn_DBPath)
	{
		QString dbPath = QFileDialog::getOpenFileName(this,
			tr("Open SQLite File"), "", tr("SQLite Files (*.db)"));
		if (!dbPath.isEmpty())
		{
			LoadSQLiteDb(dbPath);
		}
	}
	else if (curBtn == ui->btn_DisconnectDB)
	{
		m_pApp->CloaseSQLiteDb(m_pDataModel);
		ui->lineEdit_DBPath->setText("");
		ui->lineEdit_DBPath->setStyleSheet("background-color: rgb(255, 255, 255);");
	}
	else if (curBtn == ui->btn_add)
	{
		m_pDataModel->insertRow(ui->tableView_DBDataTable->selectionModel()->currentIndex());
		ui->tableView_DBDataTable->clearSelection();
	}
	else if (curBtn == ui->btn_delete)
	{
		m_pDataModel->removeRows(ui->tableView_DBDataTable->selectionModel()->selectedIndexes());
		ui->tableView_DBDataTable->clearSelection();
	}
	else if (curBtn == ui->btn_refresh)
	{
		Refresh();
	}
	else if (curBtn == ui->btn_save)
	{
		QString strErrorMsg;
		if (!m_pApp->SaveSQLiteData(m_pDataModel, strErrorMsg))
		{
			QMessageBox::critical(this, "critical", strErrorMsg);
			return;
		}
		Refresh();
	}
}

void DbScriptEditorPageForm::Refresh()
{
	LoadSQLiteDb(m_pApp->getSQLiteDbPath());
}

void DbScriptEditorPageForm::LoadSQLiteDb(const QString& dbPath)
{
	ui->lineEdit_DBPath->setText(dbPath);
	if (m_pApp->OpenSQLiteDb(m_pDataModel, dbPath))
	{
		ui->lineEdit_DBPath->setStyleSheet("background-color: rgb(0, 255, 0);");
		return;
	}
	ui->lineEdit_DBPath->setStyleSheet("background-color: rgb(255, 0, 0);");
}

void DbScriptEditorPageForm::ComboBoxCurrentIndexChangedSlot(int index)
{
	QComboBox* curComboBox = static_cast<QComboBox*>(sender());
	if (curComboBox == ui->comboBox_testCodeCol)
	{
		TestItemExcelInfo info = m_pApp->getTestItemExcelInfo();
		info.nColIndex_ItemCode = index + 1;
		info.strColName_ItemCode = curComboBox->currentText();
		m_pApp->setTestItemExcelInfo(info);
	}
	else if (curComboBox == ui->comboBox_testNameCol)
	{
		TestItemExcelInfo info = m_pApp->getTestItemExcelInfo();
		info.nColIndex_ItemName = index + 1;
		info.strColName_ItemName = curComboBox->currentText();
		m_pApp->setTestItemExcelInfo(info);
	}
}
