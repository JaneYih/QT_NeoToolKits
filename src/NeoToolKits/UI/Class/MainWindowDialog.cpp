#include "MainWindowDialog.h"
#include <QEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMessageBox>

MainWindowDialog::MainWindowDialog(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowDialogClass),
    m_pDbScriptEditPage(new NAMESPACENAME_DB_SCRIPT_EDITOR::DbScriptEditorPageForm(this)),
    m_pExcelDataUploadPage(new NAMESPACENAME_EXCEL_DATA_UPLOAD::ExcelDataUploadPageForm(this))
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

	//setAcceptDrops(true);
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

bool MainWindowDialog::OpenDbScript(const QString& dbPath)
{
	if (m_pDbScriptEditPage->IsDbScript(dbPath))
	{
		ui->actionDbScriptEditor->setChecked(true);
		m_pDbScriptEditPage->LoadSQLiteDb(dbPath);
		return true;
	}
	return false;
}

//void MainWindowDialog::dragEnterEvent(QDragEnterEvent* event)
//{
//	if (ui->actionDbScriptEditor->isChecked())
//	{
//		if (m_pDbScriptEditPage->DragEnterDbScriptFile(event))
//		{
//			event->accept();
//			return;
//		}
//	}
//	event->ignore();
//}
//
//void MainWindowDialog::dropEvent(QDropEvent* event)
//{
//	if (ui->actionDbScriptEditor->isChecked())
//	{
//		if (m_pDbScriptEditPage->DropDbScriptFile(event))
//		{
//			event->accept();
//		}
//	}
//}
