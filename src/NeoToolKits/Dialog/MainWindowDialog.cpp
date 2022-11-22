#include "MainWindowDialog.h"


MainWindowDialog::MainWindowDialog(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowDialogClass),
    m_pDbScriptEditPage(new DbScriptEditorPageForm),
    m_pExcelDataUploadPage(new ExcelDataUploadPageForm)
{
    initView();
	connect(ui->btnDbScriptEditor, &QPushButton::clicked, this, &MainWindowDialog::PageChangeBtnClickedSlot);
	connect(ui->btnExcelDataUpload, &QPushButton::clicked, this, &MainWindowDialog::PageChangeBtnClickedSlot);
    emit ui->btnExcelDataUpload->clicked(true);
}

MainWindowDialog::~MainWindowDialog()
{
    delete ui;
}

void MainWindowDialog::initView(void)
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(m_pDbScriptEditPage);
    ui->stackedWidget->addWidget(m_pExcelDataUploadPage);
}

void MainWindowDialog::PageChangeBtnClickedSlot(bool checked)
{
    QPushButton* btn = static_cast<QPushButton*>(sender());

	if (btn == ui->btnDbScriptEditor)
	{
		ui->stackedWidget->setCurrentWidget(m_pDbScriptEditPage);
	}
	else if (btn == ui->btnExcelDataUpload)
	{
		ui->stackedWidget->setCurrentWidget(m_pExcelDataUploadPage);
	}
}