#include "MysqlInfoPopDialog.h"
#include "DataTableTest.h"
#include <QMessageBox>

MysqlInfoPopDialog::MysqlInfoPopDialog(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::MysqlInfoPopDlg),
	m_pCfg(new IniOperation(QCoreApplication::applicationDirPath() + "/Database.ini"))
{
	initView();
	connect(ui->btn_test, &QPushButton::clicked, this, &MysqlInfoPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_ok, &QPushButton::clicked, this, &MysqlInfoPopDialog::PushbuttonClickedSlot);
	connect(ui->btn_cancel, &QPushButton::clicked, this, &MysqlInfoPopDialog::PushbuttonClickedSlot);
}

MysqlInfoPopDialog::~MysqlInfoPopDialog()
{
	delete ui;
}

void MysqlInfoPopDialog::initView(void)
{
	ui->setupUi(this);
	LoadCfg();
}

bool MysqlInfoPopDialog::LoadCfg()
{
	QString strPrefix = QString::fromStdWString(L"MySQL����");
	m_stTableInfo.baseInfo.type = SqlTypes::eMYSQL;
	m_stTableInfo.baseInfo.host = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"IP��ַ"), "Not Set").toString();
	m_stTableInfo.baseInfo.port = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"�˿�"), "Not Set").toString();
	m_stTableInfo.baseInfo.user = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"�û���"), "Not Set").toString();
	m_stTableInfo.baseInfo.passwd = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"����"), "Not Set").toString();
	m_stTableInfo.baseInfo.dbName= m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"���ݿ�����"), "Not Set").toString();
	m_stTableInfo.tableName = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"���ݱ�����"), "Not Set").toString();
	
	ui->lineEdit_DbIP->setText(m_stTableInfo.baseInfo.host);
	ui->lineEdit_DbPort->setText(m_stTableInfo.baseInfo.port);
	ui->lineEdit_DbUserName->setText(m_stTableInfo.baseInfo.user);
	ui->lineEdit_DbPassword->setText(m_stTableInfo.baseInfo.passwd);
	ui->lineEdit_DbName->setText(m_stTableInfo.baseInfo.dbName);
	ui->lineEdit_DbTableName->setText(m_stTableInfo.tableName);
	return true;
}

bool MysqlInfoPopDialog::SaveCfg()
{
	LoadUiData();
	QString strPrefix = QString::fromStdWString(L"MySQL����");
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"IP��ַ"), m_stTableInfo.baseInfo.host);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"�˿�"), m_stTableInfo.baseInfo.port);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"�û���"), m_stTableInfo.baseInfo.user);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"����"), m_stTableInfo.baseInfo.passwd);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"���ݿ�����"), m_stTableInfo.baseInfo.dbName);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"���ݱ�����"), m_stTableInfo.tableName);
	return true;
}

void MysqlInfoPopDialog::LoadUiData()
{
	m_stTableInfo.baseInfo.host = ui->lineEdit_DbIP->text();
	m_stTableInfo.baseInfo.port = ui->lineEdit_DbPort->text();
	m_stTableInfo.baseInfo.user = ui->lineEdit_DbUserName->text();
	m_stTableInfo.baseInfo.passwd = ui->lineEdit_DbPassword->text();
	m_stTableInfo.baseInfo.dbName = ui->lineEdit_DbName->text();
	m_stTableInfo.tableName = ui->lineEdit_DbTableName->text();
}

void MysqlInfoPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_test)
	{
		LoadUiData();
		CDataTableTest db(m_stTableInfo.baseInfo);
		std::string strErrMsg;
		if (!db.TestConnect(strErrMsg))
		{
			QMessageBox::critical(this, QString::fromStdWString(L"���Ӳ���"), QString::fromStdWString(L"����ʧ�ܣ�%1").arg(QString::fromStdString(strErrMsg)));
		}
		else
		{
			QMessageBox::critical(this, QString::fromStdWString(L"���Ӳ���"), QString::fromStdWString(L"���ӳɹ���"));
		}
	}
	else if (curBtn == ui->btn_ok)
	{
		SaveCfg();
		setResult(QDialog::Accepted);
		close();
	}
	else if (curBtn == ui->btn_cancel)
	{
		setResult(QDialog::Rejected);
		close();
	}
}

QString MysqlInfoPopDialog::infoString()
{
	return m_stTableInfo.toString();
}
