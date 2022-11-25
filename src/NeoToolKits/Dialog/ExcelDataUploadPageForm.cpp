#include "ExcelDataUploadPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "ExcelDataUploadConfigPopDialog.h"

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm)
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
}

void ExcelDataUploadPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_SelectExcelFile)
	{

	}
	else if (curBtn == ui->btn_SetDbInfo)
	{
		MysqlInfoPopDialog dlg(this);
		dlg.exec();
	}
	else if (curBtn == ui->btn_Upload)
	{
		ExcelDataUploadConfigPopDialog dlg(this);
		dlg.exec();
	}
}