#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"

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
	void initView(void);
};
