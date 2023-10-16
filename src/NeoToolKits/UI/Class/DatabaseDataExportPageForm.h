#pragma once

#include <QWidget>
#include "ui_DatabaseDataExportPageForm.h"
#include "DatabaseDataExport_def.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DatabaseDataExportPageFormClass; };
QT_END_NAMESPACE

class MysqlInfoPopDialog;
namespace NAMESPACENAME_DATABASE_DATA_EXPORT
{
	class DatabaseDataExportModel;
	class DatabaseDataExportDelegate;
	class DatabaseDataExportApp;
	class DatabaseDataExportPageForm : public QWidget
	{
		Q_OBJECT

	public:
		DatabaseDataExportPageForm(QWidget* parent = nullptr);
		~DatabaseDataExportPageForm();

	protected:
		virtual void showEvent(QShowEvent* event) override;

	public slots:
		void PushbuttonClickedSlot(bool checked = false);
		void onExportFinish(bool result, const QString& errorMsg);

	private:
		void initView(void);
		void EditTableViewByDbConnect();

	private:
		Ui::DatabaseDataExportPageFormClass* ui;
		MysqlInfoPopDialog* m_pMysqlInfoDlg;
		DatabaseDataExportApp* m_pApp;
		DatabaseDataExportModel* m_pDataModel;
		DatabaseDataExportDelegate* m_pDataDelegate;
		bool m_bFirstShowData;
	};

};
