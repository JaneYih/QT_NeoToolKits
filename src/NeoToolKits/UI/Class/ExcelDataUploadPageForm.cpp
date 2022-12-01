#include "ExcelDataUploadPageForm.h"
#include "ExcelDataUploadConfigPopDialog.h"
#include <QFileDialog>
#include <QDebug>

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm),
	m_app(new ExcelDataUploadApp(this))
{
	initView();
	connect(ui->btn_SelectExcelFile, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_SetDbInfo, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_Upload, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
}

ExcelDataUploadPageForm::~ExcelDataUploadPageForm()
{
	delete m_pMysqlInfoDlg;
	delete m_app;
	delete ui;
}

void ExcelDataUploadPageForm::initView(void)
{
	ui->setupUi(this);
	ui->lineEdit_ExcelFile->setText(m_app->getExcelFileName());

	m_pMysqlInfoDlg = new MysqlInfoPopDialog(m_app->getSqlTableInfoPointer(), this, false);
	m_pMysqlInfoDlg->setIniFileName(m_app->getIniFileName());
	m_pMysqlInfoDlg->setIniPrefix(m_app->getIniSqlCfgPrefix());
	m_pMysqlInfoDlg->LoadIniCfg();
	ui->lineEdit_DbInfo->setText(m_app->getSqlTableInfoPointer()->toString());
}

void ExcelDataUploadPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_SelectExcelFile)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Excel File"), "", tr("Excel Files (*.xlsx)"));
		if (!fileName.isNull())
		{
			QStringList ExcelColumns = m_app->LoadExcelColumns(fileName);
			if (!ExcelColumns.isEmpty())
			{
				ui->lineEdit_ExcelFile->setText(m_app->getExcelFileName());
				qDebug() << ExcelColumns;
			}
		}
	}
	else if (curBtn == ui->btn_SetDbInfo)
	{
		m_pMysqlInfoDlg->UpdataUiData();
		m_pMysqlInfoDlg->exec();
		ui->lineEdit_DbInfo->setText(m_app->getSqlTableInfoPointer()->toString());
	}
	else if (curBtn == ui->btn_Upload)
	{
		ExcelDataUploadConfigPopDialog dlg(this);
		dlg.exec();
	}
}
