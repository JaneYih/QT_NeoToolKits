#pragma once

#include <QWidget>
#include "ui_ExcelDataUploadPageForm.h"
#include "ExcelDataUpload_def.h"

class MysqlInfoPopDialog;

namespace NAMESPACENAME_EXCEL_DATA_UPLOAD
{
	class ExcelDataUploadApp;
	class ExcelDataUploadDataModel;
	class ExcelDataUploadDataDelegate;

	class ExcelDataUploadPageForm : public QWidget
	{
		Q_OBJECT

	public:
		ExcelDataUploadPageForm(QWidget* parent = nullptr);
		~ExcelDataUploadPageForm();

	protected:
		virtual void showEvent(QShowEvent* event) override;

	public slots:
		void PushbuttonClickedSlot(bool checked = false);

	private:
		void initView(void);
		void EditTableViewByExcel(const QString& excelFileName);
		void EditTableViewByDbConnect();

	private:
		Ui::ExcelDataUploadPageForm* ui;
		ExcelDataUploadApp* m_pApp;
		MysqlInfoPopDialog* m_pMysqlInfoDlg;
		ExcelDataUploadDataModel* m_pDataModel;
		ExcelDataUploadDataDelegate* m_pDataDelegate;
		bool m_bFirstShowData;
	};
};
