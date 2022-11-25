#pragma once

#include <QDialog>
#include "ui_MysqlInfoPopDialog.h"

class MysqlInfoPopDialog  : public QDialog
{
	Q_OBJECT

public:
	MysqlInfoPopDialog(QWidget *parent = nullptr);
	~MysqlInfoPopDialog();

private:
	void initView(void);

private:
	Ui::MysqlInfoPopDlg* ui;
};
