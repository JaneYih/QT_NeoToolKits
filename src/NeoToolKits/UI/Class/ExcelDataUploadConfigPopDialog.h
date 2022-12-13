#pragma once

#include <QDialog>
#include "ui_ExcelDataUploadConfigPopDialog.h"
#include "ExcelDataUploadApp.h"

class ExcelDataUploadConfigPopDialog  : public QDialog
{
	Q_OBJECT

public:
	ExcelDataUploadConfigPopDialog(QWidget *parent, ExcelDataUploadApp* pApp);
	~ExcelDataUploadConfigPopDialog();

protected:
	virtual void closeEvent(QCloseEvent* event);

private:
	void initView(void);
	void ClearDisplay(int rowCount);
	void UploadingView(bool uploading);

private:
	Ui::ExcelDataUploadConfigPopDlg* ui;
	ExcelDataUploadApp* const m_pApp;

public slots:
	void PushbuttonClickedSlot(bool checked = false);
	void DisplayItemSlot(const QString& text, int count, int countMax);
	void DisplayFinishSlot();
};
