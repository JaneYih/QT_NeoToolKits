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

	/*��������в�����رմ��ڣ����ǿ��Ե����ť��ֹ����󣬲��ܹر�*/

private:
	Ui::ExcelDataUploadConfigPopDlg* ui;
};
