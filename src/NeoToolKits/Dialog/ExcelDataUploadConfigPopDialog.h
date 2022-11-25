#pragma once

#include <QDialog>
#include "ui_ExcelDataUploadConfigPopDialog.h"

class ExcelDataUploadConfigPopDialog  : public QDialog
{
	Q_OBJECT

public:
	ExcelDataUploadConfigPopDialog(QWidget *parent = nullptr);
	~ExcelDataUploadConfigPopDialog();

private:
	void initView(void);

	/*导入过程中不允许关闭窗口，但是可以点击按钮终止导入后，才能关闭*/

private:
	Ui::ExcelDataUploadConfigPopDlg* ui;
};
