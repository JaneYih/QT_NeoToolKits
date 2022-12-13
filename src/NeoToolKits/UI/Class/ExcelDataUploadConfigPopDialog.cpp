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
	connect(m_pApp, &ExcelDataUploadApp::toDisplayItem, this, &ExcelDataUploadConfigPopDialog::DisplayItemSlot, Qt::BlockingQueuedConnection);
	connect(m_pApp, &ExcelDataUploadApp::toDisplayFinish, this, &ExcelDataUploadConfigPopDialog::DisplayFinishSlot);
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
	ui->lineEdit_StartLine->setText("2");
	ui->lineEdit_Count->setValidator(new QIntValidator(0, iExcelUploadRowCountMax, this));
	ui->lineEdit_Count->setText(QString("%1").arg(iExcelUploadRowCountMax));
	ui->checkBox_ErrorStop->setChecked(true);
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(iExcelUploadRowCountMax);
	ui->progressBar->setValue(0);
	ui->progressBar->setFormat("%p% (%v/%m)");// %p 百分比 //%v 当前值 //%m 总值
}

void ExcelDataUploadConfigPopDialog::closeEvent(QCloseEvent* event)
{
	if (m_pApp->getUploading())
	{
		QMessageBox::warning(this, "warning", QString::fromStdWString(L"请先停止再关闭窗口"));
		event->ignore();
	}
}

void ExcelDataUploadConfigPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_StartOrStop)
	{
		if (m_pApp->getUploading())
		{
			ui->btn_StartOrStop->setEnabled(false);
			m_pApp->StopUpload();
		}
		else
		{
			ExcelDataUploadConfig stUploadConfig;
			stUploadConfig.iRowCountMax = ui->lineEdit_CountMax->text().toInt();
			stUploadConfig.iRowStartRowIndex = ui->lineEdit_StartLine->text().toInt();
			stUploadConfig.iRowCount = ui->lineEdit_Count->text().toInt();
			stUploadConfig.strProductionOrderID = ui->lineEdit_ProductionOrderID->text();
			stUploadConfig.bErrorStop = ui->checkBox_ErrorStop->checkState() == Qt::Checked;
			if (stUploadConfig.strProductionOrderID.isEmpty())
			{
				QMessageBox::warning(this, "warning", QString::fromStdWString(L"工单号不能为空"));
				return;
			}
			m_pApp->setUploadConfig(stUploadConfig);
			ClearDisplay(stUploadConfig.iRowCount);
			m_pApp->StartUpload();
			UploadingView(true);
		}
	}
}

void ExcelDataUploadConfigPopDialog::DisplayItemSlot(const QString& text, int count, int countMax)
{
	ui->listWidget->addItem(text);
	ui->listWidget->scrollToBottom();
	ui->progressBar->setMaximum(countMax);
	ui->progressBar->setValue(count);
}

void ExcelDataUploadConfigPopDialog::DisplayFinishSlot()
{
	UploadingView(false);
}

void ExcelDataUploadConfigPopDialog::ClearDisplay(int rowCount)
{
	ui->listWidget->clear();
	ui->progressBar->setMaximum(rowCount);
	ui->progressBar->setValue(0);
}

void ExcelDataUploadConfigPopDialog::UploadingView(bool uploading)
{
	ui->lineEdit_StartLine->setEnabled(!uploading);
	ui->lineEdit_Count->setEnabled(!uploading);
	ui->lineEdit_ProductionOrderID->setEnabled(!uploading);
	ui->checkBox_ErrorStop->setEnabled(!uploading);
	ui->btn_StartOrStop->setEnabled(true);
	ui->btn_StartOrStop->setText(uploading ? QString::fromStdWString(L"停止") : QString::fromStdWString(L"开始"));
}