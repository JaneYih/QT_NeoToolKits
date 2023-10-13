#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindowDialog.h"
#include "DbScriptEditorPageForm.h"
#include "ExcelDataUploadPageForm.h"
#include "DatabaseDataExportPageForm.h"
#include <QMap>

class MainWindowDialog : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowDialog(QWidget *parent = nullptr);
    ~MainWindowDialog();

    bool OpenDbScript(const QString& dbPath);

//protected:
//	virtual void dragEnterEvent(QDragEnterEvent* event) override;
//	virtual void dropEvent(QDropEvent* event) override;

private:
	void initView(void);

public slots:
	void PageChangeActionToggledSlot(bool toggled);
 
private:
    Ui::MainWindowDialogClass* ui;
    QMap<QAction*, QWidget*> m_mapToolsPageDictionary;
    NAMESPACENAME_DB_SCRIPT_EDITOR::DbScriptEditorPageForm* m_pDbScriptEditPage;
    NAMESPACENAME_EXCEL_DATA_UPLOAD::ExcelDataUploadPageForm* m_pExcelDataUploadPage;
    NAMESPACENAME_DATABASE_DATA_EXPORT::DatabaseDataExportPageForm* m_pDatabaseDataExport;
};
