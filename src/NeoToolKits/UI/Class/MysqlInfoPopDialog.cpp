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
	m_strIniPrefixDef(QString::fromStdWString(L"MySQL����"))
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
	m_strIniFileName = fileName;
}

QString MysqlInfoPopDialog::getIniFileName() const
{
	return m_bUseDefaultIni ? m_strIniFileNameDef : m_strIniFileName;
}

void MysqlInfoPopDialog::setIniPrefix(const QString& prefix)
{
	m_strIniPrefix = prefix;
}

QString MysqlInfoPopDialog::getIniPrefix() const
{
	return m_bUseDefaultIni ? m_strIniPrefixDef : m_strIniPrefix;
}

bool MysqlInfoPopDialog::LoadIniCfg()
{
	IniOperation ini(getIniFileName());
	QString prefix = getIniPrefix();
	m_pstTableInfo->baseInfo.type = SqlTypes::eMYSQL;
	m_pstTableInfo->baseInfo.host = ini.ReadValue(prefix, QString::fromStdWString(L"IP��ַ"), "Not Set").toString();
	m_pstTableInfo->baseInfo.port = ini.ReadValue(prefix, QString::fromStdWString(L"�˿�"), "Not Set").toString();
	m_pstTableInfo->baseInfo.user = ini.ReadValue(prefix, QString::fromStdWString(L"�û���"), "Not Set").toString();
	m_pstTableInfo->baseInfo.passwd = ini.ReadValue(prefix, QString::fromStdWString(L"����"), "Not Set").toString();
	m_pstTableInfo->baseInfo.dbName= ini.ReadValue(prefix, QString::fromStdWString(L"���ݿ�����"), "Not Set").toString();
	m_pstTableInfo->tableName = ini.ReadValue(prefix, QString::fromStdWString(L"���ݱ�����"), "Not Set").toString();
	return true;
}

bool MysqlInfoPopDialog::SaveIniCfg(const SqlTableInfo& info)
{
	IniOperation ini(getIniFileName());
	QString prefix = getIniPrefix();
	ini.WriteValue(prefix, QString::fromStdWString(L"IP��ַ"), info.baseInfo.host);
	ini.WriteValue(prefix, QString::fromStdWString(L"�˿�"), info.baseInfo.port);
	ini.WriteValue(prefix, QString::fromStdWString(L"�û���"), info.baseInfo.user);
	ini.WriteValue(prefix, QString::fromStdWString(L"����"), info.baseInfo.passwd);
	ini.WriteValue(prefix, QString::fromStdWString(L"���ݿ�����"), info.baseInfo.dbName);
	ini.WriteValue(prefix, QString::fromStdWString(L"���ݱ�����"), info.tableName);
	return true;
}

void MysqlInfoPopDialog::GetUiData(SqlTableInfo& info)
{
	info.baseInfo.host = ui->lineEdit_DbIP->text();
	info.baseInfo.port = ui->lineEdit_DbPort->text();
	info.baseInfo.user = ui->lineEdit_DbUserName->text();
	info.baseInfo.passwd = ui->lineEdit_DbPassword->text();
	info.baseInfo.dbName = ui->lineEdit_DbName->text();
	info.tableName = ui->lineEdit_DbTableName->text();
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
		SqlTableInfo tempInfo;
		GetUiData(tempInfo);
		tempInfo.baseInfo.type = SqlTypes::eMYSQL;
		CDataTableTest db(tempInfo.baseInfo);
		std::string strErrMsg;
		if (!db.TestConnect(strErrMsg))
		{
			QMessageBox::critical(this, QString::fromStdWString(L"���Ӳ���"), QString::fromStdWString(L"����ʧ�ܣ�%1").arg(QString::fromLocal8Bit(strErrMsg.c_str())));
		}
		else
		{
			QMessageBox::information(this, QString::fromStdWString(L"���Ӳ���"), QString::fromStdWString(L"���ӳɹ���"));
		}
	}
	else if (curBtn == ui->btn_ok)
	{
		GetUiData(*m_pstTableInfo);
		SaveIniCfg(*m_pstTableInfo);
		setResult(QDialog::Accepted);
		close();
	}
	else if (curBtn == ui->btn_cancel)
	{
		setResult(QDialog::Rejected);
		close();
	}
}
