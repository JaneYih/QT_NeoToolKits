#include "DbScriptEditorPageForm.h"
#include <QDebug>
#include "DbScriptEditorApp.h"
#include <QFileDialog>
#include <QComboBox>

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DbScriptEditorPageForm::DbScriptEditorPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::DbScriptEditorPageForm),
	m_pApp(new DbScriptEditorApp(this)),
	m_bFirstShowData(true)
{
	Q_ASSERT(m_pApp);
	initView();
	connect(ui->btn_ExcelPath, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
	connect(ui->btn_DBPath, &QPushButton::clicked, this, &DbScriptEditorPageForm::PushbuttonClickedSlot);
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

		m_bFirstShowData = false;
	}
}

void DbScriptEditorPageForm::InitComboBoxItems(const TestItemExcelInfo& info)
{
	ui->comboBox_testCodeCol->clear();
	ui->comboBox_testCodeCol->addItems(info.listExcelColumns);
	int curIndex = ui->comboBox_testCodeCol->count() >= info.nTestItemCodeExcelColumnIndex
		? info.nTestItemCodeExcelColumnIndex - 1
		: -1;
	ui->comboBox_testCodeCol->setCurrentIndex(curIndex);
	emit ui->comboBox_testCodeCol->currentIndexChanged(curIndex);

	ui->comboBox_testNameCol->clear();
	ui->comboBox_testNameCol->addItems(info.listExcelColumns);
	curIndex = ui->comboBox_testNameCol->count() >= info.nTestItemNameExcelColumnIndex
					? info.nTestItemNameExcelColumnIndex - 1
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
			ui->lineEdit_DBPath->setText(dbPath);
			if (m_pApp->TestSqliteDb(dbPath, "model_testlist"))
			{
				ui->lineEdit_DBPath->setStyleSheet("background-color: rgb(0, 255, 0);");
				return;
			}
			ui->lineEdit_DBPath->setStyleSheet("background-color: rgb(255, 0, 0);");
		}
	}
	else if (curBtn == ui->btn_add)
	{

	}
	else if (curBtn == ui->btn_delete)
	{

	}
	else if (curBtn == ui->btn_refresh)
	{

	}
	else if (curBtn == ui->btn_save)
	{

	}
}

void DbScriptEditorPageForm::ComboBoxCurrentIndexChangedSlot(int index)
{
	QComboBox* curComboBox = static_cast<QComboBox*>(sender());
	if (curComboBox == ui->comboBox_testCodeCol)
	{
		TestItemExcelInfo info = m_pApp->getTestItemExcelInfo();
		info.nTestItemCodeExcelColumnIndex = index + 1;
		info.strTestItemCodeExcelColumnName = curComboBox->currentText();
		m_pApp->setTestItemExcelInfo(info);
	}
	else if (curComboBox == ui->comboBox_testNameCol)
	{
		TestItemExcelInfo info = m_pApp->getTestItemExcelInfo();
		info.nTestItemNameExcelColumnIndex = index + 1;
		info.strTestItemNameExcelColumnName = curComboBox->currentText();
		m_pApp->setTestItemExcelInfo(info);
	}
}
