#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindowDialog.h"
#include "DbScriptEditorPageForm.h"
#include "ExcelDataUploadPageForm.h"
#include <QMap>

class MainWindowDialog : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowDialog(QWidget *parent = nullptr);
    ~MainWindowDialog();

private:
	void initView(void);

public slots:
	void PageChangeActionToggledSlot(bool toggled);
 
private:
    Ui::MainWindowDialogClass* ui;
    QMap<QAction*, QWidget*> m_mapToolsPageDictionary;
    NAMESPACENAME_DB_SCRIPT_EDITOR::DbScriptEditorPageForm* m_pDbScriptEditPage;
    NAMESPACENAME_EXCEL_DATA_UPLOAD::ExcelDataUploadPageForm* m_pExcelDataUploadPage;
};
