#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"
#include "MysqlInfoPopDialog.h"
#include "ExcelDataUploadApp.h"
#include "ExcelDataUploadDataModel.h"
#include "ExcelDataUploadDataDelegate.h"

class ExcelDataUploadPageForm  : public QWidget
{
	Q_OBJECT

public:
	ExcelDataUploadPageForm(QWidget *parent = nullptr);
	~ExcelDataUploadPageForm();

protected:
	virtual void showEvent(QShowEvent* event);

public slots:
	void PushbuttonClickedSlot(bool checked = false);

private:
	void initView(void);
	void EditTableViewByExcel(const QString& excelFileName);
	void EditTableViewByDbConnect();

private:
	Ui::ExcelDataUploadPageForm* ui;
	ExcelDataUploadApp* m_app;
	MysqlInfoPopDialog* m_pMysqlInfoDlg;
	ExcelDataUploadDataModel* m_pDataModel;
	ExcelDataUploadDataDelegate* m_pDataDelegate;
	bool m_bFirstShowData;
};
