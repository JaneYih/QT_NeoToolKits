#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "ExcelDataUploadApp.h"

class ExcelDataUploadPageForm  : public QWidget
{
	Q_OBJECT

public:
	ExcelDataUploadPageForm(QWidget *parent = nullptr);
	~ExcelDataUploadPageForm();

public slots:
	void PushbuttonClickedSlot(bool checked = false);

private:
	void initView(void);

private:
	Ui::ExcelDataUploadPageForm* ui;
	ExcelDataUploadApp* m_app;
	MysqlInfoPopDialog* m_pMysqlInfoDlg;
};
