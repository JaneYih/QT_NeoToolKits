#include "MysqlInfoPopDialog.h"
#include "DataTableTest.h"
#include <QMessageBox>
#include "IniOperation.h"

MysqlInfoPopDialog::MysqlInfoPopDialog(SqlTableInfo* pTableInfo, QWidget* parent, bool bUseDefaultIni)
	: QDialog(parent),
	ui(new Ui::MysqlInfoPopDlg),
	m_pstTableInfo(pTableInfo),
	m_bUseDefaultIni(bUseDefaultIni),
	m_strIniFileNameDef(QCoreApplication::applicationDirPath() + "/Database.ini"),
	m_strIniPrefixDef(QString::fromStdWString(L"MySQL配置"))
{
	Q_ASSERT(m_pstTableInfo);
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

void MysqlInfoPopDialog::setUseDefaultIni(bool enable)
{
	m_bUseDefaultIni = enable;
}

void MysqlInfoPopDialog::setIniFileName(const QString& fileName)
{
	m_strIniFileName = m_bUseDefaultIni ? m_strIniFileNameDef : fileName;
}

void MysqlInfoPopDialog::setIniPrefix(const QString& prefix)
{
	m_strIniPrefix = m_bUseDefaultIni ? m_strIniPrefixDef : prefix;
}

bool MysqlInfoPopDialog::LoadIniCfg()
{
	IniOperation ini(m_strIniFileName);
	m_pstTableInfo->baseInfo.type = SqlTypes::eMYSQL;
	m_pstTableInfo->baseInfo.host = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"IP地址"), "Not Set").toString();
	m_pstTableInfo->baseInfo.port = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"端口"), "Not Set").toString();
	m_pstTableInfo->baseInfo.user = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"用户名"), "Not Set").toString();
	m_pstTableInfo->baseInfo.passwd = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"密码"), "Not Set").toString();
	m_pstTableInfo->baseInfo.dbName= ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"数据库名称"), "Not Set").toString();
	m_pstTableInfo->tableName = ini.ReadValue(m_strIniPrefix, QString::fromStdWString(L"数据表名称"), "Not Set").toString();
	return true;
}

bool MysqlInfoPopDialog::SaveIniCfg()
{
	IniOperation ini(m_strIniFileName);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"IP地址"), m_pstTableInfo->baseInfo.host);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"端口"), m_pstTableInfo->baseInfo.port);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"用户名"), m_pstTableInfo->baseInfo.user);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"密码"), m_pstTableInfo->baseInfo.passwd);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"数据库名称"), m_pstTableInfo->baseInfo.dbName);
	ini.WriteValue(m_strIniPrefix, QString::fromStdWString(L"数据表名称"), m_pstTableInfo->tableName);
	return true;
}

void MysqlInfoPopDialog::GetUiData()
{
	m_pstTableInfo->baseInfo.host = ui->lineEdit_DbIP->text();
	m_pstTableInfo->baseInfo.port = ui->lineEdit_DbPort->text();
	m_pstTableInfo->baseInfo.user = ui->lineEdit_DbUserName->text();
	m_pstTableInfo->baseInfo.passwd = ui->lineEdit_DbPassword->text();
	m_pstTableInfo->baseInfo.dbName = ui->lineEdit_DbName->text();
	m_pstTableInfo->tableName = ui->lineEdit_DbTableName->text();
}

void MysqlInfoPopDialog::UpdataUiData()
{
	ui->lineEdit_DbIP->setText(m_pstTableInfo->baseInfo.host);
	ui->lineEdit_DbPort->setText(m_pstTableInfo->baseInfo.port);
	ui->lineEdit_DbUserName->setText(m_pstTableInfo->baseInfo.user);
	ui->lineEdit_DbPassword->setText(m_pstTableInfo->baseInfo.passwd);
	ui->lineEdit_DbName->setText(m_pstTableInfo->baseInfo.dbName);
	ui->lineEdit_DbTableName->setText(m_pstTableInfo->tableName);
}

void MysqlInfoPopDialog::PushbuttonClickedSlot(bool checked)
{
	QPushButton* curBtn = static_cast<QPushButton*>(sender());
	if (curBtn == ui->btn_test)
	{
		GetUiData();
		CDataTableTest db(m_pstTableInfo->baseInfo);
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
