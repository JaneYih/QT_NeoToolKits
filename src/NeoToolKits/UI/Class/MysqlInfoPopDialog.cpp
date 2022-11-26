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
	QString strPrefix = QString::fromStdWString(L"MySQL配置");
	m_stTableInfo.baseInfo.type = SqlTypes::eMYSQL;
	m_stTableInfo.baseInfo.host = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"IP地址"), "Not Set").toString().toStdString();
	m_stTableInfo.baseInfo.port = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"端口"), "Not Set").toInt();
	m_stTableInfo.baseInfo.user = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"用户名"), "Not Set").toString().toStdString();
	m_stTableInfo.baseInfo.passwd = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"密码"), "Not Set").toString().toStdString();
	
	m_stTableInfo.baseInfo.db = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"数据库名称"), "Not Set").toString().toStdString();
	m_stTableInfo.table = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"数据表名称"), "Not Set").toString().toStdString();
	
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
	QString strPrefix = QString::fromStdWString(L"MySQL配置");
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"IP地址"), QString::fromStdString(m_stTableInfo.baseInfo.host));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"端口"), m_stTableInfo.baseInfo.port);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"用户名"), QString::fromStdString(m_stTableInfo.baseInfo.user));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"密码"), QString::fromStdString(m_stTableInfo.baseInfo.passwd));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"数据库名称"), QString::fromStdString(m_stTableInfo.baseInfo.db));
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"数据表名称"), QString::fromStdString(m_stTableInfo.table));
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
		QMessageBox::critical(this, QString::fromStdWString(L"连接测试"), db.TestConnect() ? QString::fromStdWString(L"连接成功！") : QString::fromStdWString(L"连接失败！"));
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
	return QString::fromStdWString(L"IP地址:%1;端口:%2;用户名:%3;数据库名:%4;表名:%5")
		.arg(QString::fromStdString(m_stTableInfo.baseInfo.host))
		.arg(m_stTableInfo.baseInfo.port)
		.arg(QString::fromStdString(m_stTableInfo.baseInfo.user))
		.arg(QString::fromStdString(m_stTableInfo.baseInfo.db))
		.arg(QString::fromStdString(m_stTableInfo.table));
}
