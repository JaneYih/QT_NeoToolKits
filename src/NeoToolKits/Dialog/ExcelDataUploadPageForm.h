#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"

class ExcelDataUploadPageForm  : public QWidget
{
	Q_OBJECT

public:
	ExcelDataUploadPageForm(QWidget *parent = nullptr);
	~ExcelDataUploadPageForm();

private:
	Ui::ExcelDataUploadPageForm* ui;
	void initView(void);
};
