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
	m_stTableInfo.baseInfo.host = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"IP��ַ"), "Not Set").toString().toStdString();
	m_stTableInfo.baseInfo.port = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"�˿�"), "Not Set").toInt();
	m_stTableInfo.baseInfo.user = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"�û���"), "Not Set").toString().toStdString();
	m_stTableInfo.baseInfo.passwd = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"����"), "Not Set").toString().toStdString();
	
	m_stTableInfo.baseInfo.db = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"���ݿ�����"), "Not Set").toString().toStdString();
	m_stTableInfo.table = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"���ݱ�����"), "Not Set").toString().toStdString();
	
	ui->lineEdit_DbIP->setText(QString::fromStdString(m_stTableInfo.baseInfo.host));
	ui->lineEdit_DbPort->setText(QString("%1").arg(m_stTableInfo.baseInfo.port));
	ui->lineEdit_DbUserName->setText(QString::fromStdString(m_stTableInfo.baseInfo.user));
	ui->lineEdit_DbPassword->setText(QString::fromStdString(m_stTableInfo.baseInfo.passwd));
	ui->lineEdit_DbName->setText(QString::fromStdString(m_stTableInfo.baseInfo.db));
	ui->lineEdit_DbTableName->setText(QString::fromStdString(m_stTableInfo.table));
	return true;
}

bool MysqlInfoPopDialog::SaveCfg()
{
	LoadUiData();
	QString strPrefix = QString::fromStdWString(L"MySQL����");
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"IP��ַ"), QString::fromStdString(m_stTableInfo.baseInfo.host));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"�˿�"), m_stTableInfo.baseInfo.port);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"�û���"), QString::fromStdString(m_stTableInfo.baseInfo.user));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"����"), QString::fromStdString(m_stTableInfo.baseInfo.passwd));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"���ݿ�����"), QString::fromStdString(m_stTableInfo.baseInfo.db));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"���ݱ�����"), QString::fromStdString(m_stTableInfo.table));
	return true;
}

void MysqlInfoPopDialog::LoadUiData()
{
	QString DbIP = ui->lineEdit_DbIP->text();
	QString DbPort = ui->lineEdit_DbPort->text();
	QString DbUserName = ui->lineEdit_DbUserName->text();
	QString DbPassword = ui->lineEdit_DbPassword->text();
	QString DbName = ui->lineEdit_DbName->text();
	QString DbTableName = ui->lineEdit_DbTableName->text();

	m_stTableInfo.baseInfo.host = DbIP.toStdString();
	m_stTableInfo.baseInfo.port = DbPort.toInt();
	m_stTableInfo.baseInfo.user = DbUserName.toStdString();
	m_stTableInfo.baseInfo.passwd = DbPassword.toStdString();
	m_stTableInfo.baseInfo.db = DbName.toStdString();
	m_stTableInfo.table = DbTableName.toStdString();
}

void MysqlInfoPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_test)
	{
		LoadUiData();
		CDataTableTest db(m_stTableInfo.baseInfo);
		QMessageBox::critical(this, QString::fromStdWString(L"���Ӳ���"), db.TestConnect() ? QString::fromStdWString(L"���ӳɹ���") : QString::fromStdWString(L"����ʧ�ܣ�"));
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

QString MysqlInfoPopDialog::infoString() const
{
	return QString::fromStdWString(L"IP��ַ:%1;�˿�:%2;�û���:%3;���ݿ���:%4;����:%5")
		.arg(QString::fromStdString(m_stTableInfo.baseInfo.host))
		.arg(m_stTableInfo.baseInfo.port)
		.arg(QString::fromStdString(m_stTableInfo.baseInfo.user))
		.arg(QString::fromStdString(m_stTableInfo.baseInfo.db))
		.arg(QString::fromStdString(m_stTableInfo.table));
}
