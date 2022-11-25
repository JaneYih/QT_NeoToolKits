#include "ExcelDataUploadConfigPopDialog.h"

ExcelDataUploadConfigPopDialog::ExcelDataUploadConfigPopDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ExcelDataUploadConfigPopDlg)
{
	initView();
}

ExcelDataUploadConfigPopDialog::~ExcelDataUploadConfigPopDialog()
{
	delete ui;
}

void ExcelDataUploadConfigPopDialog::initView(void)
{
	ui->setupUi(this);
}
