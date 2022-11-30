#include "ExcelDataUploadPageForm.h"
#include "ExcelDataUploadConfigPopDialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


//ini prefixs
const QString s_ini_prefix_excel = "Excel";

//ini keys
const QString s_ini_key_excelFile = "ExcelFlie";

ExcelDataUploadPageForm::ExcelDataUploadPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::ExcelDataUploadPageForm),
	m_strIniFileName(QCoreApplication::applicationDirPath() + "/ExcelDataUploadTool.ini"),
	m_pCfg(new IniOperation(m_strIniFileName)),
	m_strExcelFileName(m_pCfg->ReadValue(s_ini_prefix_excel, s_ini_key_excelFile, "").toString())
{
	m_pMysqlInfoDlg = new MysqlInfoPopDialog(this, false);
	m_pMysqlInfoDlg->setIniFileName(m_strIniFileName);
	m_pMysqlInfoDlg->setIniPrefix(QString::fromStdWString(L"MySQL����"));
	m_pMysqlInfoDlg->LoadIniCfg();

	initView();
	connect(ui->btn_SelectExcelFile, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_SetDbInfo, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
	connect(ui->btn_Upload, &QPushButton::clicked, this, &ExcelDataUploadPageForm::PushbuttonClickedSlot);
}

ExcelDataUploadPageForm::~ExcelDataUploadPageForm()
{
	delete m_pCfg;
	delete ui;
}

void ExcelDataUploadPageForm::initView(void)
{
	ui->setupUi(this);
	ui->lineEdit_ExcelFile->setText(m_strExcelFileName);
	ui->lineEdit_DbInfo->setText(m_pMysqlInfoDlg->getSqlTableInfo().toString());
}

void ExcelDataUploadPageForm::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_SelectExcelFile)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open Excel File"), "", tr("Excel Files (*.xls *.xlsx)"));
		if (!fileName.isNull())
		{
			QXlsx::Document xlsx(fileName);
			if (xlsx.load()) // load excel file
			{
				m_strExcelFileName = fileName;
				m_pCfg->WriteValue(s_ini_prefix_excel, s_ini_key_excelFile, m_strExcelFileName);
				ui->lineEdit_ExcelFile->setText(m_strExcelFileName);

				QXlsx::Workbook* workBook = xlsx.workbook();
				QXlsx::Worksheet* workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(0));
				CellRange* cellRange = &(workSheet->dimension());
				int rowCount = cellRange->rowCount();
				int columnCount = cellRange->columnCount();
				for (int col = 1; col <= columnCount; col++)
				{
					Cell* cell = xlsx.cellAt(1, col);
					if (cell != NULL)
					{
						QVariant var = cell->readValue();
						qDebug() << QString("col %1:").arg(col) << var.toString();
					}
				}
			}
			else
			{
				QMessageBox::critical(this, "error", "excel open fail:" + fileName);
			}
		}
	}
	else if (curBtn == ui->btn_SetDbInfo)
	{
		m_pMysqlInfoDlg->UpdataUiData();
		m_pMysqlInfoDlg->exec();
		ui->lineEdit_DbInfo->setText(m_pMysqlInfoDlg->getSqlTableInfo().toString());
	}
	else if (curBtn == ui->btn_Upload)
	{
		ExcelDataUploadConfigPopDialog dlg(this);
		dlg.exec();
	}
}