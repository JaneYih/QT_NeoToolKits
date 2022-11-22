#include "ExcelDataUploadPageForm.h"

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm)
{
	initView();
}

ExcelDataUploadPageForm::~ExcelDataUploadPageForm()
{
	delete ui;
}

void ExcelDataUploadPageForm::initView(void)
{
	ui->setupUi(this);

}