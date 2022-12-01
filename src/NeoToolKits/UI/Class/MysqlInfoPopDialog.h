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

	void setUseDefaultIni(bool enable);
	void setIniFileName(const QString& fileName);
	void setIniPrefix(const QString& prefix);
	bool LoadIniCfg();
	void UpdataUiData();

public slots:
	void PushbuttonClickedSlot(bool checked = false);

private:
	void initView(void);
	void GetUiData();
	bool SaveIniCfg();

private:
	Ui::MysqlInfoPopDlg* ui;
	SqlTableInfo* m_pstTableInfo;

	bool m_bUseDefaultIni;
	QString m_strIniFileName;
	QString m_strIniPrefix;
	QString m_strIniFileNameDef;
	QString m_strIniPrefixDef;
};

