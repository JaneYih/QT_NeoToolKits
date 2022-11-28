#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "IniOperation.h"

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

	QString m_strIniFileName;
	IniOperation* m_pCfg;
	QString m_strExcelFileName;
	MysqlInfoPopDialog* m_pMysqlInfoDlg;
};
