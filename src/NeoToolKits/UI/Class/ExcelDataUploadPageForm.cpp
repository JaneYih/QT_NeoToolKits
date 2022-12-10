#include "ExcelDataUploadPageForm.h"
#include "ExcelDataUploadConfigPopDialog.h"
#include <QFileDialog>
#include <QDebug>
#include "DataTableTest.h"

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm),
	m_app(new ExcelDataUploadApp(this)),
	m_pDataModel(new ExcelDataUploadDataModel),
	m_pDataDelegate(new ExcelDataUploadDataDelegate),
	m_bFirstShowData(true)
{
	Q_ASSERT(m_app);
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
	delete m_app;
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
		m_pMysqlInfoDlg = new MysqlInfoPopDialog(m_app->getSqlTableInfoPointer(), this, false);
		m_pMysqlInfoDlg->setIniFileName(m_app->getIniFileName());
		m_pMysqlInfoDlg->setIniPrefix(m_app->getIniSqlCfgPrefix());
		m_pMysqlInfoDlg->LoadIniCfg();

		EditTableViewByDbConnect();  //加载数据库的字段信息
		EditTableViewByExcel(m_app->getExcelFileName()); //加载Excel的列信息

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
		m_app->setExcelFileName(excelFileName);
		ui->lineEdit_ExcelFile->setText(excelFileName);

		QStringList ExcelColumns = m_app->LoadExcelColumns(excelFileName);
		m_pDataModel->initData(ExcelColumns);
		if (ExcelColumns.isEmpty())
		{
			ui->lineEdit_ExcelFile->setStyleSheet("background-color: rgb(255, 0, 0);");
			return;
		}
		ui->lineEdit_ExcelFile->setStyleSheet("background-color: rgb(0, 255, 0);");
	}
}

void ExcelDataUploadPageForm::EditTableViewByDbConnect()
{
	ui->lineEdit_DbInfo->setText(m_app->getSqlTableInfoPointer()->toString());

	CDataTableTest db(m_app->getSqlTableInfoPointer()->baseInfo);
	std::list<std::string> fields;
	if (db.GetTableFullFields(m_app->getSqlTableInfoPointer()->tableName.toUtf8(), fields))
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
		QVector<ExcelDataUploadInfo> dataMap = m_pDataModel->getData();
		SqlTableInfo dbInfo = *(m_app->getSqlTableInfoPointer());

		ExcelDataUploadConfigPopDialog dlg(this);
		dlg.exec();
	}
}
