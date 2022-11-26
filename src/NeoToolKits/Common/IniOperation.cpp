#include "IniOperation.h"
#include <QTextCodec>

#ifdef Q_OS_WIN
#include "windows.h"
#endif

IniOperation::IniOperation(QString fileName)
	: m_strFileName(fileName)
{
	m_pCfg = new QSettings(m_strFileName, QSettings::IniFormat);
	//m_pCfg->setIniCodec(QTextCodec::codecForName("GB2312"));
}

IniOperation::~IniOperation()
{
	delete m_pCfg;
}

QVariant IniOperation::ReadValue(const QString& prefix, const QString& key, const QVariant& defaultValue)
{
#ifdef Q_OS_WIN
	wchar_t buffer[300] = { 0 };
	std::wstring strPrefix = prefix.toStdWString();
	std::wstring strKey = key.toStdWString();
	std::wstring strFileName = m_strFileName.toStdWString();
	std::wstring strdefaultValue = defaultValue.toString().toStdWString();
	DWORD bufferNum = GetPrivateProfileString(strPrefix.data(), strKey.data(), strdefaultValue.data(),
		(LPWSTR)buffer, 300, strFileName.data());
	return QString::fromWCharArray(buffer);
#else
	m_pCfg->beginGroup(prefix);
	QVariant value = m_pCfg->value(key, defaultValue);
	m_pCfg->endGroup();
	return value;
#endif
}

void IniOperation::WriteValue(const QString& prefix, const QString& key, const QVariant& value)
{
#ifdef Q_OS_WIN
	std::wstring strPrefix = prefix.toStdWString();
	std::wstring strKey = key.toStdWString();
	std::wstring strValue = value.toString().toStdWString();
	std::wstring strFileName = m_strFileName.toStdWString();
	WritePrivateProfileString(strPrefix.data(), strKey.data(), strValue.data(), strFileName.data());
#else
	m_pCfg->beginGroup(prefix);
	m_pCfg->setValue(key, value);
	m_pCfg->endGroup();
#endif
}


QSettings* IniOperation::getCfg() const
{
	return m_pCfg;
}

QStringList IniOperation::allKeys(const QString& prefix)
{
	m_pCfg->beginGroup(prefix);
	QStringList keys = m_pCfg->allKeys();
	m_pCfg->endGroup();
	return keys;
}