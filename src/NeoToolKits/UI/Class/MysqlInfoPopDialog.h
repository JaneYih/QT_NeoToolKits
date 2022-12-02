#pragma once

#include <QDialog>
#include "ui_MysqlInfoPopDialog.h"
#include "Database_def.h"

class MysqlInfoPopDialog  : public QDialog
{
	Q_OBJECT

public:
	explicit MysqlInfoPopDialog(SqlTableInfo* pTableInfo, QWidget* parent = nullptr, bool bUseDefaultIni = true);
	~MysqlInfoPopDialog();

	bool LoadIniCfg();
	void UpdataUiData();

	void setUseDefaultIni(bool enable);
	void setIniFileName(const QString& fileName);
	void setIniPrefix(const QString& prefix);

public slots:
	void PushbuttonClickedSlot(bool checked = false);

private:
	void initView(void);
	void GetUiData(SqlTableInfo& info);
	bool SaveIniCfg(const SqlTableInfo& info);
	QString getIniFileName() const;
	QString getIniPrefix() const;

private:
	Ui::MysqlInfoPopDlg* ui;
	SqlTableInfo* m_pstTableInfo;

	bool m_bUseDefaultIni;
	QString m_strIniFileName;
	QString m_strIniPrefix;
	QString m_strIniFileNameDef;
	QString m_strIniPrefixDef;
};

