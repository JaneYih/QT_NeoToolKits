#include "ExcelDataUploadPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "ExcelDataUploadConfigPopDialog.h"
#include <QFileDialog>

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm),
	m_strExcelFileName("")
{
	initView();
	connect(ui->btn_SelectExcelFile, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_SetDbInfo, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_Upload, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
}

ExcelDataUploadPageForm::~ExcelDataUploadPageForm()
{
	delete ui;
}

void ExcelDataUploadPageForm::initView(void)
{
	ui->setupUi(this);
	ui->lineEdit_ExcelFile->setText(m_strExcelFileName);
}

void ExcelDataUploadPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_SelectExcelFile)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Excel File"), "", tr("Excel Files (*.xls *.xlsx)"));
		if (!fileName.isNull())
		{
			m_strExcelFileName = fileName;
			ui->lineEdit_ExcelFile->setText(m_strExcelFileName);
		}
	}
	else if (curBtn == ui->btn_SetDbInfo)
	{
		MysqlInfoPopDialog dlg(this);
		if (dlg.exec() == QDialog::Accepted)
		{
			ui->lineEdit_DbInfo->setText(dlg.infoString());
		}
	}
	else if (curBtn == ui->btn_Upload)
	{
		ExcelDataUploadConfigPopDialog dlg(this);
		dlg.exec();
	}
}