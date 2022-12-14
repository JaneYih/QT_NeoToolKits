#include "ExcelDataUploadPageForm.h"
#include "ExcelDataUploadConfigPopDialog.h"
#include <QFileDialog>
#include <QDebug>
#include "ExcelDataUploadDbOperate.h"
#include <QMessageBox>
#include "MysqlInfoPopDialog.h"
#include "ExcelDataUploadApp.h"
#include "ExcelDataUploadDataModel.h"
#include "ExcelDataUploadDataDelegate.h"

using namespace NAMESPACENAME_EXCEL_DATA_UPLOAD;

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm),
	m_pApp(new ExcelDataUploadApp(this)),
	m_pDataModel(new ExcelDataUploadDataModel),
	m_pDataDelegate(new ExcelDataUploadDataDelegate),
	m_bFirstShowData(true)
{
	Q_ASSERT(m_pApp);
	Q_ASSERT(m_pDataModel);
	Q_ASSERT(m_pDataDelegate);
	initView();
	connect(ui->btn_SelectExcelFile, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_SetDbInfo, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_Upload, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
}

ExcelDataUploadPageForm::~ExcelDataUploadPageForm()
{
	delete m_pMysqlInfoDlg;
	delete m_pDataDelegate;
	delete m_pDataModel;
	delete m_pApp;
	delete ui;
}

void ExcelDataUploadPageForm::initView(void)
{
	ui->setupUi(this);
}

void ExcelDataUploadPageForm::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
	if (m_bFirstShowData)
	{
		m_pMysqlInfoDlg = new MysqlInfoPopDialog(m_pApp->getSqlTableInfoPointer(), this, false);
		m_pMysqlInfoDlg->LoadIniCfg(m_pApp->getIniFileName(), m_pApp->getIniSqlCfgPrefix());

		EditTableViewByDbConnect();  //加载数据库的字段信息
		EditTableViewByExcel(m_pApp->getExcelFileName()); //加载Excel的列信息

		ui->tableView->setModel(m_pDataModel);
		ui->tableView->setItemDelegateForColumn(1, m_pDataDelegate);
		ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		//ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
		//ui->tableView->horizontalHeader()->setStretchLastSection(true);
		ui->tableView->setAlternatingRowColors(true);
		ui->tableView->show();

		m_bFirstShowData = false;
	}
}

void ExcelDataUploadPageForm::EditTableViewByExcel(const QString& excelFileName)
{
	if (!excelFileName.isNull())
	{
		m_pApp->setExcelFileName(excelFileName);
		ui->lineEdit_ExcelFile->setText(excelFileName);

		QStringList ExcelColumns = m_pApp->LoadExcelColumns(excelFileName);
		m_pDataModel->initData(ExcelColumns);
		if (ExcelColumns.isEmpty())
		{
			QString strTemp(ui->lineEdit_ExcelFile->text());
			ui->lineEdit_ExcelFile->setText(QString("%1-->%2").arg(strTemp).arg("excel open fail!"));
			ui->lineEdit_ExcelFile->setStyleSheet("background-color: rgb(255, 0, 0);");
			return;
		}
		ui->lineEdit_ExcelFile->setStyleSheet("background-color: rgb(0, 255, 0);");
	}
}

void ExcelDataUploadPageForm::EditTableViewByDbConnect()
{
	ui->lineEdit_DbInfo->setText(m_pApp->getSqlTableInfoPointer()->toString());

	ExcelDataUploadDbOperate db(*(m_pApp->getSqlTableInfoPointer()));
	std::list<std::string> fields;
	if (db.GetTableFullFields(m_pApp->getSqlTableInfoPointer()->tableName.toUtf8(), fields))
	{
		QStringList options;
		options.push_back("");
		foreach (auto var, fields)
		{
			options.push_back(QString::fromStdString(var));
		}
		m_pDataDelegate->setComboCtrlOptions(options);
		ui->lineEdit_DbInfo->setStyleSheet("background-color: rgb(0, 255, 0);");
	}
	else
	{
		QString strErrMsg;
		if (!db.test(strErrMsg))
		{
			QString strTemp(ui->lineEdit_DbInfo->text());
			ui->lineEdit_DbInfo->setText(QString("%1-->%2").arg(strTemp).arg(strErrMsg));
		}
		m_pDataDelegate->setComboCtrlOptions(QStringList());
		ui->lineEdit_DbInfo->setStyleSheet("background-color: rgb(255, 0, 0);");
	}
}

void ExcelDataUploadPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_SelectExcelFile)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Excel File"), "", tr("Excel Files (*.xlsx)"));
		EditTableViewByExcel(fileName);
	}
	else if (curBtn == ui->btn_SetDbInfo)
	{
		m_pMysqlInfoDlg->UpdataUiData();
		m_pMysqlInfoDlg->exec();
		EditTableViewByDbConnect();
	}
	else if (curBtn == ui->btn_Upload)
	{
		QVector<ExcelDataUploadInfo> dataMap;
		foreach (auto var, m_pDataModel->getData())
		{
			if (var.isValid())
			{
				dataMap.push_back(var);
			}
		}
		if (dataMap.isEmpty())
		{
			QMessageBox::critical(this, "critical", QString::fromStdWString(L"请先选择数据库字段") );
			return;
		}
		m_pApp->setDataMap(dataMap);
		ExcelDataUploadConfigPopDialog dlg(this, m_pApp);
		dlg.exec();
	}
}
