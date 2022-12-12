#include "ExcelDataUploadConfigPopDialog.h"
#include <QIntValidator>
#include <QMessageBox>

ExcelDataUploadConfigPopDialog::ExcelDataUploadConfigPopDialog(QWidget* parent, ExcelDataUploadApp* pApp)
	: QDialog(parent),
	m_pApp(pApp),
	ui(new Ui::ExcelDataUploadConfigPopDlg)
{
	Q_ASSERT(m_pApp);
	initView();
	connect(ui->btn_StartOrStop, &QPushButton::clicked, this, &ExcelDataUploadConfigPopDialog::PushbuttonClickedSlot);
}

ExcelDataUploadConfigPopDialog::~ExcelDataUploadConfigPopDialog()
{
	delete ui;
}

void ExcelDataUploadConfigPopDialog::initView(void)
{
	ui->setupUi(this);
	int iExcelUploadRowCountMax = m_pApp->LoadExcelRowCount();
	ui->lineEdit_CountMax->setText(QString("%1").arg(iExcelUploadRowCountMax));
	ui->lineEdit_StartLine->setValidator(new QIntValidator(1, iExcelUploadRowCountMax, this));
	ui->lineEdit_StartLine->setText("1");
	ui->lineEdit_Count->setValidator(new QIntValidator(0, iExcelUploadRowCountMax, this));
	ui->lineEdit_Count->setText(QString("%1").arg(iExcelUploadRowCountMax));
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(iExcelUploadRowCountMax);
	ui->progressBar->setValue(0);
}

void ExcelDataUploadConfigPopDialog::closeEvent(QCloseEvent* event)
{
	if (m_pApp->getIsUploading())
	{
		QMessageBox::warning(this, "warning", "Uploading, forbidden to close.");
		event->ignore();
	}
}

void ExcelDataUploadConfigPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_StartOrStop)
	{
		ExcelDataUploadConfig stUploadConfig;
		stUploadConfig.iRowCountMax = ui->lineEdit_CountMax->text().toInt();
		stUploadConfig.iRowStartRowIndex = ui->lineEdit_StartLine->text().toInt();
		stUploadConfig.iRowCount = ui->lineEdit_Count->text().toInt();
		stUploadConfig.strProductionOrderID = ui->label_ProductionOrderID->text();
		stUploadConfig.bErrorStop = ui->checkBox_ErrorStop->checkState() == Qt::Checked;
		m_pApp->setUploadConfig(stUploadConfig);

		ui->progressBar->setMaximum(stUploadConfig.iRowCount);
		ui->progressBar->setValue(5);

		ui->listWidget->clear();
		ui->listWidget->addItem("111");
		ui->listWidget->addItem("333");
		ui->listWidget->addItem("222");
		ui->listWidget->addItem("444");
		ui->listWidget->addItem("666");
		ui->listWidget->addItem("555");
		ui->listWidget->addItem("888");
		ui->listWidget->addItem("777");
		ui->listWidget->scrollToBottom();
	}
}
