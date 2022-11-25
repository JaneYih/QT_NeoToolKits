#include "MysqlInfoPopDialog.h"

MysqlInfoPopDialog::MysqlInfoPopDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::MysqlInfoPopDlg)
{
	initView();
}

MysqlInfoPopDialog::~MysqlInfoPopDialog()
{
	delete ui;
}

void MysqlInfoPopDialog::initView(void)
{
	ui->setupUi(this);
}
