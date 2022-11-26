#pragma once

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QCoreApplication>

class IniOperation : QObject
{
public:
	IniOperation(QString fileName = QCoreApplication::applicationDirPath() + "/config.ini");
	~IniOperation();

	QSettings* getCfg() const;
	QVariant ReadValue(const QString& prefix, const QString& key, const QVariant& defaultValue = QVariant());
	void WriteValue(const QString& prefix, const QString& key, const QVariant& value);
	
	QStringList allKeys(const QString& prefix);

private:
	QSettings* m_pCfg;
	QString m_strFileName;
};

