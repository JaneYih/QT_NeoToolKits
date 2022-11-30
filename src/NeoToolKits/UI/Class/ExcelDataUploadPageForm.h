#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"
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
	Ui::ExcelDataUploadPageForm* ui;
	ExcelDataUploadApp* m_app;

	void initView(void);
};
