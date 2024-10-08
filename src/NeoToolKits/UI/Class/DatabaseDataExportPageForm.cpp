#include "DatabaseDataExportPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "DatabaseDataExportModel.h"
#include "DatabaseDataExportApp.h"
#include "DatabaseDataExportDelegate.h"
#include "DatabaseDataExportDbOperate.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QProgressDialog>
#include <QDesktopServices>
#include "msgbox.h"
#include "IniOperation.h"

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
	connect(m_pApp, &DatabaseDataExportApp::toExportFinish, this, &DatabaseDataExportPageForm::onExportFinish);
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
	
		ExportConfig exportConfig = m_pApp->getExportConfig();
		ui->lineEdit_WorkOrderID->setText(exportConfig.dataIndexCondition.workOrderID);
		ui->lineEdit_BoxNumberStart->setText(exportConfig.dataIndexCondition.BoxNumberStart);
		ui->lineEdit_BoxNumberEnd->setText(exportConfig.dataIndexCondition.BoxNumberEnd);
		ui->lineEdit_TrayNoStart->setText(exportConfig.dataIndexCondition.TrayNoStart);
		ui->lineEdit_TrayNoEnd->setText(exportConfig.dataIndexCondition.TrayNoEnd);

		ui->tableView->setModel(m_pDataModel);
		for each (auto var in m_pDataDelegate->GetDelegateColumnIndexs())
		{
			ui->tableView->setItemDelegateForColumn(var, m_pDataDelegate);
		}
		ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
		ui->tableView->horizontalHeader()->setStretchLastSection(true);
		//ui->tableView->setAlternatingRowColors(true);
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
		QVector<ExportDataUnit> options;
		IniOperation ini(m_pApp->getIniFileName());
		for each (auto var in fields.FieldListValue)
		{
			QString dbKey = QString::fromStdString(var);
			QString excelTitle = dbKey;
			bool onOff = false;
			bool emptyUnexport = false;
			QString optionInfo = ini.ReadValue(m_pApp->getSqlTableInfoPointer()->tableName, dbKey, "").toString();
			if (!optionInfo.isEmpty() && optionInfo.contains(','))
			{
				auto infos = optionInfo.split(',');
				if (infos.size() >= 3)
				{
					onOff = QVariant(infos[0]).toBool();
					emptyUnexport = QVariant(infos[1]).toBool();
					excelTitle = infos[2];
				}
			}
			options.push_back(ExportDataUnit(dbKey, excelTitle, onOff, emptyUnexport));
		}
		m_pDataModel->setData(options);
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
		exportCfg.dataIndexCondition.TrayNoStart = ui->lineEdit_TrayNoStart->text();
		exportCfg.dataIndexCondition.TrayNoEnd = ui->lineEdit_TrayNoEnd->text();
		IniOperation ini(m_pApp->getIniFileName());
		for each (auto var in m_pDataModel->getData())
		{
			ini.WriteValue(m_pApp->getSqlTableInfoPointer()->tableName, var.DbKey, QString("%1,%2,%3").arg(var.bExport).arg(var.bAllEmptyUnexport).arg(var.ExcelTitle));
			if (var.bExport || var.bAllEmptyUnexport)
			{
				exportCfg.exportFields.push_back(var);
			}
		}
		QString excelFileName = QFileDialog::getSaveFileName(this,
			QString::fromStdWString(L"保存文件"),
			QString("%1/data_%2").arg(QDir::homePath()).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hh_mm_ss")),
			"Excel File(*.xlsx)");
		if (!excelFileName.isEmpty())
		{
			exportCfg.excelName = excelFileName;
			ui->btn_Export->setEnabled(false);
			m_pApp->StartExport(exportCfg);
		}
	}
}

void DatabaseDataExportPageForm::onExportFinish(bool result, const QString& errorMsg)
{
	m_pApp->m_progressDialog->close();
	if (result)
	{
		msgBox::show("Tip", QString::fromStdWString(L"导出成功！！"), msgBox::msgBoxType::information);
	}
	else
	{
		msgBox::show("error", QString::fromStdWString(L"导出失败：%1").arg(errorMsg), msgBox::msgBoxType::critical);
	}
	ui->btn_Export->setEnabled(true);
}
