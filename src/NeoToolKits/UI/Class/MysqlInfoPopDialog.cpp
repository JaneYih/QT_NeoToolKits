#include "MysqlInfoPopDialog.h"
#include "DataTableTest.h"
#include <QMessageBox>
#include "IniOperation.h"

MysqlInfoPopDialog::MysqlInfoPopDialog(QWidget* parent, bool bUseDefaultIni, SqlTableInfo* pTableInfo)
	: m_bUseDefaultIni(bUseDefaultIni),
	QDialog(parent),
	ui(new Ui::MysqlInfoPopDlg)
{
	if (m_bUseDefaultIni)
	{
		setIniFileName(QCoreApplication::applicationDirPath() + "/Database.ini");
		setIniPrefix(QString::fromStdWString(L"MySQL配置"));
	}

	if (pTableInfo)
	{
		m_stTableInfo = *pTableInfo;
	}
	else
	{
		LoadIniCfg();
		m_stTableInfo.baseInfo.type = SqlTypes::eMYSQL;
	}

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
	UpdataUiData();
}

SqlTableInfo MysqlInfoPopDialog::getSqlTableInfo() const
{
	return m_stTableInfo;
}

void MysqlInfoPopDialog::setIniFileName(const QString& fileName)
{
	m_strIniFileName = fileName;
}

void MysqlInfoPopDialog::setIniPrefix(const QString& prefix)
{
	m_strIniPrefix = prefix;
}

bool MysqlInfoPopDialog::LoadIniCfg()
{
	IniOperation ini(m_strIniFileName);
	m_stTableInfo.baseInfo.host = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"IP地址"), "Not Set").toString();
	m_stTableInfo.baseInfo.port = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"端口"), "Not Set").toString();
	m_stTableInfo.baseInfo.user = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"用户名"), "Not Set").toString();
	m_stTableInfo.baseInfo.passwd = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"密码"), "Not Set").toString();
	m_stTableInfo.baseInfo.dbName= ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"数据库名称"), "Not Set").toString();
	m_stTableInfo.tableName = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"数据表名称"), "Not Set").toString();
	return true;
}

bool MysqlInfoPopDialog::SaveIniCfg()
{
	IniOperation ini(m_strIniFileName);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"IP地址"), m_stTableInfo.baseInfo.host);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"端口"), m_stTableInfo.baseInfo.port);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"用户名"), m_stTableInfo.baseInfo.user);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"密码"), m_stTableInfo.baseInfo.passwd);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"数据库名称"), m_stTableInfo.baseInfo.dbName);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"数据表名称"), m_stTableInfo.tableName);
	return true;
}

void MysqlInfoPopDialog::GetUiData()
{
	m_stTableInfo.baseInfo.host = ui->lineEdit_DbIP->text();
	m_stTableInfo.baseInfo.port = ui->lineEdit_DbPort->text();
	m_stTableInfo.baseInfo.user = ui->lineEdit_DbUserName->text();
	m_stTableInfo.baseInfo.passwd = ui->lineEdit_DbPassword->text();
	m_stTableInfo.baseInfo.dbName = ui->lineEdit_DbName->text();
	m_stTableInfo.tableName = ui->lineEdit_DbTableName->text();
}

void MysqlInfoPopDialog::UpdataUiData()
{
	ui->lineEdit_DbIP->setText(m_stTableInfo.baseInfo.host);
	ui->lineEdit_DbPort->setText(m_stTableInfo.baseInfo.port);
	ui->lineEdit_DbUserName->setText(m_stTableInfo.baseInfo.user);
	ui->lineEdit_DbPassword->setText(m_stTableInfo.baseInfo.passwd);
	ui->lineEdit_DbName->setText(m_stTableInfo.baseInfo.dbName);
	ui->lineEdit_DbTableName->setText(m_stTableInfo.tableName);
}

void MysqlInfoPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_test)
	{
		GetUiData();
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
		GetUiData();
		SaveIniCfg();
		setResult(QDialog::Accepted);
		close();
	}
	else if (curBtn == ui->btn_cancel)
	{
		setResult(QDialog::Rejected);
		close();
	}
}
