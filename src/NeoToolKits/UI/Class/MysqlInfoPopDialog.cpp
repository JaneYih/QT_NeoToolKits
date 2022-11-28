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
	m_stTableInfo.baseInfo.host = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"IP地址"), "Not Set").toString();
	m_stTableInfo.baseInfo.port = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"端口"), "Not Set").toString();
	m_stTableInfo.baseInfo.user = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"用户名"), "Not Set").toString();
	m_stTableInfo.baseInfo.passwd = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"密码"), "Not Set").toString();
	m_stTableInfo.baseInfo.dbName= m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"数据库名称"), "Not Set").toString();
	m_stTableInfo.tableName = m_pCfg->ReadValue(strPrefix, QString::fromStdWString(L"数据表名称"), "Not Set").toString();
	
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
	QString strPrefix = QString::fromStdWString(L"MySQL配置");
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"IP地址"), m_stTableInfo.baseInfo.host);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"端口"), m_stTableInfo.baseInfo.port);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"用户名"), m_stTableInfo.baseInfo.user);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"密码"), m_stTableInfo.baseInfo.passwd);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"数据库名称"), m_stTableInfo.baseInfo.dbName);
	m_pCfg->WriteValue(strPrefix, QString::fromStdWString(L"数据表名称"), m_stTableInfo.tableName);
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
			QMessageBox::critical(this, QString::fromStdWString(L"连接测试"), QString::fromStdWString(L"连接失败：%1").arg(QString::fromStdString(strErrMsg)));
		}
		else
		{
			QMessageBox::critical(this, QString::fromStdWString(L"连接测试"), QString::fromStdWString(L"连接成功！"));
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
