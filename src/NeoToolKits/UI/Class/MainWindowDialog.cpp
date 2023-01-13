#include "MainWindowDialog.h"

MainWindowDialog::MainWindowDialog(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowDialogClass),
    m_pDbScriptEditPage(new NAMESPACENAME_DB_SCRIPT_EDITOR::DbScriptEditorPageForm),
    m_pExcelDataUploadPage(new NAMESPACENAME_EXCEL_DATA_UPLOAD::ExcelDataUploadPageForm)
{
	ui->setupUi(this);
    initView();
	connect(ui->actionDbScriptEditor, &QAction::toggled, this, &MainWindowDialog::PageChangeActionToggledSlot);
	connect(ui->actionExcelDataUpload, &QAction::toggled, this, &MainWindowDialog::PageChangeActionToggledSlot);
	ui->actionDbScriptEditor->setChecked(true);
}

MainWindowDialog::~MainWindowDialog()
{
    delete ui;
}

void MainWindowDialog::initView(void)
{
	m_mapToolsPageDictionary.clear();
    ui->stackedWidget->addWidget(m_pDbScriptEditPage);
	m_mapToolsPageDictionary[ui->actionDbScriptEditor] = m_pDbScriptEditPage;
    ui->stackedWidget->addWidget(m_pExcelDataUploadPage);
	m_mapToolsPageDictionary[ui->actionExcelDataUpload] = m_pExcelDataUploadPage;
}

void MainWindowDialog::PageChangeActionToggledSlot(bool toggled)
{
	QAction* curAct = static_cast<QAction*>(sender());
	if (toggled)
	{
		QList<QAction*> actions = ui->menuTools->actions();
		foreach(QAction * act, actions)
		{
			if (curAct == act)
			{
				QWidget* w = m_mapToolsPageDictionary[curAct];
				if (w != nullptr)
				{
					ui->stackedWidget->setCurrentWidget(w);
					ui->stackedWidget->setStatusTip(curAct->text());
				}
				act->setChecked(true);
			}
			else
			{
				act->setChecked(false);
			}
		}
	}
	else
	{
		if (m_mapToolsPageDictionary[curAct] == ui->stackedWidget->currentWidget())
		{
			curAct->setChecked(true);
		}
	}
}