#include "DatabaseDataExportPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "DatabaseDataExportModel.h"
#include "DatabaseDataExportApp.h"
#include "DatabaseDataExportDelegate.h"
#include "DatabaseDataExportDbOperate.h"
#include <QMessageBox>
#include <QDebug>

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

DatabaseDataExportPageForm::DatabaseDataExportPageForm(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::DatabaseDataExportPageFormClass())
	, m_pApp(new DatabaseDataExportApp(this))
	, m_pDataModel(new DatabaseDataExportModel)
	, m_pDataDelegate(new DatabaseDataExportDelegate)
	, m_bFirstShowData(true)
{
	Q_ASSERT(m_pApp);
	Q_ASSERT(m_pDataModel);
	Q_ASSERT(m_pDataDelegate);
	ui->setupUi(this);
	initView();
	connect(ui->btn_SetDbInfo, &QPushButton::clicked, this, &DatabaseDataExportPageForm::PushbuttonClickedSlot);
	connect(ui->btn_Export, &QPushButton::clicked, this, &DatabaseDataExportPageForm::PushbuttonClickedSlot);
}

DatabaseDataExportPageForm::~DatabaseDataExportPageForm()
{
	delete ui;
}

void DatabaseDataExportPageForm::initView(void)
{

}

void DatabaseDataExportPageForm::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
	if (m_bFirstShowData)
	{
		m_pMysqlInfoDlg = new MysqlInfoPopDialog(m_pApp->getSqlTableInfoPointer(), this, false);
		m_pMysqlInfoDlg->LoadIniCfg(m_pApp->getIniFileName(), m_pApp->getIniSqlCfgPrefix());

		EditTableViewByDbConnect();  //加载数据库的字段信息
	
		ui->tableView->setModel(m_pDataModel);
		ui->tableView->setItemDelegateForColumn(m_pDataDelegate->GetDelegateColumnIndex(), m_pDataDelegate);
		ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
		ui->tableView->horizontalHeader()->setStretchLastSection(true);
		ui->tableView->setAlternatingRowColors(true);
		ui->tableView->show();

		m_bFirstShowData = false;
	}
}

void DatabaseDataExportPageForm::EditTableViewByDbConnect()
{
	ui->lineEdit_DbInfo->setText(m_pApp->getSqlTableInfoPointer()->toString());

	DatabaseDataExportDbOperate db(*(m_pApp->getSqlTableInfoPointer()));
	FieldList fields;
	if (db.GetTableFullFields(m_pApp->getSqlTableInfoPointer()->tableName.toUtf8(), fields))
	{
		QStringList options;
		for each (auto var in fields.FieldListValue)
		{
			options.push_back(QString::fromStdString(var));
		}
		m_pDataModel->initData(options);
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
		ui->lineEdit_DbInfo->setStyleSheet("background-color: rgb(255, 0, 0);");
	}
}


void DatabaseDataExportPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_SetDbInfo)
	{
		m_pMysqlInfoDlg->UpdataUiData();
		m_pMysqlInfoDlg->exec();
		EditTableViewByDbConnect();
	}
	else if (curBtn == ui->btn_Export)
	{
		ExportConfig exportCfg;
		exportCfg.dataIndexCondition.workOrderID = ui->lineEdit_WorkOrderID->text();
		exportCfg.dataIndexCondition.BoxNumberStart = ui->lineEdit_BoxNumberStart->text();
		exportCfg.dataIndexCondition.BoxNumberEnd = ui->lineEdit_BoxNumberEnd->text();
		for each (auto var in m_pDataModel->getData())
		{
			if (var.bExport)
			{
				exportCfg.exportFields.push_back(var);
			}
		}

	}
}
