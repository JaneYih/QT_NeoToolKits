#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindowDialog.h"
#include "DbScriptEditorPageForm.h"
#include "ExcelDataUploadPageForm.h"

class MainWindowDialog : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowDialog(QWidget *parent = nullptr);
    ~MainWindowDialog();

private:
	void initView(void);

public slots:
	void PageChangeBtnClickedSlot(bool checked = false);

private:
    Ui::MainWindowDialogClass* ui;
    DbScriptEditorPageForm* m_pDbScriptEditPage;
    ExcelDataUploadPageForm* m_pExcelDataUploadPage;
};
