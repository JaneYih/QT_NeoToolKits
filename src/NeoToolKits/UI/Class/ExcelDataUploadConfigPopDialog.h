#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QAction>
#include <QMenu>
#include "ui_ExcelDataUploadConfigPopDialog.h"
#include "ExcelDataUploadApp.h"

class ExcelDataUploadConfigPopDialog  : public QDialog
{
	Q_OBJECT

public:
	ExcelDataUploadConfigPopDialog(QWidget *parent, ExcelDataUploadApp* pApp);
	~ExcelDataUploadConfigPopDialog();

protected:
	virtual void closeEvent(QCloseEvent* event) override;
	virtual bool eventFilter(QObject* obj, QEvent* event) override;
	virtual void reject() override;

private:
	void initView(void);
	void ClearDisplay(int rowCount);
	void UploadingView(bool uploading);

private:
	Ui::ExcelDataUploadConfigPopDlg* ui;
	ExcelDataUploadApp* const m_pApp;
	QAction* m_pItemCopyAct;
	QMenu* m_pContextMenu;

public slots:
	void PushbuttonClickedSlot(bool checked = false);
	void DisplayItemSlot(const QString& text, int count, int countMax);
	void DisplayFinishSlot();
	void DisplayItemCopySlot(bool checked = false);
};
