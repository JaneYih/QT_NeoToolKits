#include "ExcelDataUploadConfigPopDialog.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QGuiApplication>
#include <QClipboard>
#include "ExcelDataUploadApp.h"

using namespace NAMESPACENAME_EXCEL_DATA_UPLOAD;

ExcelDataUploadConfigPopDialog::ExcelDataUploadConfigPopDialog(QWidget* parent, ExcelDataUploadApp* pApp)
	: QDialog(parent),
	ui(new Ui::ExcelDataUploadConfigPopDlg),
	m_pApp(pApp)
{
	Q_ASSERT(m_pApp);
	ui->setupUi(this);
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
	int iExcelUploadRowCountMax = m_pApp->LoadExcelRowCount();
	ui->lineEdit_CountMax->setText(QString("%1").arg(iExcelUploadRowCountMax));
	ui->lineEdit_StartLine->setValidator(new QIntValidator(2, iExcelUploadRowCountMax, this));
	ui->lineEdit_StartLine->setText("2");
	ui->lineEdit_Count->setValidator(new QIntValidator(1, iExcelUploadRowCountMax, this));
	ui->lineEdit_Count->setText(QString("%1").arg(iExcelUploadRowCountMax));
	ui->lineEdit_ProductionOrderID->setValidator(new QRegExpValidator(QRegExp("^[0-9]{5,10}$"), this));
	ui->checkBox_ErrorStop->setChecked(true);
	ui->comboBox_Options->clear();
	ui->comboBox_Options->addItem(QString::fromStdWString(L"更新数据"));
	ui->comboBox_Options->addItem(QString::fromStdWString(L"插入数据"));
	ui->comboBox_Options->setCurrentText(QString::fromStdWString(L"更新数据"));
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(iExcelUploadRowCountMax);
	ui->progressBar->setValue(0);
	ui->progressBar->setFormat("%p% (%v/%m)");// %p 百分比 //%v 当前值 //%m 总值

	m_pItemCopyAct = new QAction(QString::fromStdWString(L"复制"), this);
	m_pItemCopyAct->setShortcuts(QKeySequence::Copy);
	connect(m_pItemCopyAct, &QAction::triggered, this, &ExcelDataUploadConfigPopDialog::DisplayItemCopySlot);

	m_pItemSelectAllAct = new QAction(QString::fromStdWString(L"全选"), this);
	m_pItemSelectAllAct->setShortcuts(QKeySequence::SelectAll);
	connect(m_pItemSelectAllAct, &QAction::triggered, this, &ExcelDataUploadConfigPopDialog::DisplayItemSelectAllSlot);

	m_pContextMenu = new QMenu(this);
	m_pContextMenu->addAction(m_pItemCopyAct);
	m_pContextMenu->addAction(m_pItemSelectAllAct);

	ui->listWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
	ui->listWidget->installEventFilter(this);
}

void ExcelDataUploadConfigPopDialog::closeEvent(QCloseEvent* event)
{
	if (m_pApp->getUploading())
	{
		QMessageBox::warning(this, "warning", QString::fromStdWString(L"请先停止再关闭窗口"));
		event->ignore();
	}
}

void ExcelDataUploadConfigPopDialog::reject()
{
	close();
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
			stUploadConfig.pApp = m_pApp;
			stUploadConfig.iRowCountMax = ui->lineEdit_CountMax->text().toInt();
			stUploadConfig.iRowStartRowIndex = ui->lineEdit_StartLine->text().toInt();
			stUploadConfig.iRowCount = ui->lineEdit_Count->text().toInt();
			stUploadConfig.strProductionOrderID = ui->lineEdit_ProductionOrderID->text();
			stUploadConfig.bErrorStop = ui->checkBox_ErrorStop->checkState() == Qt::Checked;

			QString strCurrentOptions = ui->comboBox_Options->currentText();
			stUploadConfig.eOpentions = strCurrentOptions == QString::fromStdWString(L"插入数据")
				? UploadOptions::InsertCommand 
				: UploadOptions::UpdateCommand;

			QString strWarningMsg;
			if (!stUploadConfig.isValid(strWarningMsg))
			{
				QMessageBox::warning(this, "warning", strWarningMsg);
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
	QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
	item->setText(text);
	item->setToolTip(text);
	ui->listWidget->addItem(item);
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

bool ExcelDataUploadConfigPopDialog::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui->listWidget)
	{
		if (event->type() == QEvent::ContextMenu)
		{
			QContextMenuEvent* contextMenuEvent = static_cast<QContextMenuEvent*>(event);
			if (contextMenuEvent->reason() == QContextMenuEvent::Mouse)
			{
				QModelIndexList selection = ui->listWidget->selectionModel()->selectedRows();
				if (selection.count() > 0)
				{
					m_pContextMenu->popup(contextMenuEvent->globalPos());
					return true;
				}
			}
		}
	}
	return QWidget::eventFilter(obj, event);
}

void ExcelDataUploadConfigPopDialog::DisplayItemCopySlot(bool checked)
{
	//方法一
	//QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
	//selectedItems.at(0)->text();

	//方法二
	QModelIndexList selection = ui->listWidget->selectionModel()->selectedRows();
	QString text;
	foreach (auto var, selection)
	{
		text += var.data().toString() + "\r\n";
	}
	if (!text.isEmpty())
	{
		text = text.left(text.length() - 2);
		QGuiApplication::clipboard()->setText(text);
	}
}

void ExcelDataUploadConfigPopDialog::DisplayItemSelectAllSlot(bool checked)
{
	int rowCount = ui->listWidget->count();
	for (int row = 0; row < rowCount; ++row)
	{
		QListWidgetItem* item = ui->listWidget->item(row);
		if (item)
		{
			item->setSelected(true);
		}
	}
}
