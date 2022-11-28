#pragma once

#include <QDialog>
#include "ui_MysqlInfoPopDialog.h"
#include "Database_def.h"

class MysqlInfoPopDialog  : public QDialog
{
	Q_OBJECT

public:
	explicit MysqlInfoPopDialog(QWidget* parent = nullptr, bool bUseDefaultIni = true, SqlTableInfo* pTableInfo = nullptr);
	~MysqlInfoPopDialog();

	void setIniFileName(const QString& fileName);
	void setIniPrefix(const QString& prefix);
	bool LoadIniCfg();
	void UpdataUiData();
	SqlTableInfo getSqlTableInfo() const;

public slots:
	void PushbuttonClickedSlot(bool checked = false);

private:
	void initView(void);
	void GetUiData();
	bool SaveIniCfg();

private:
	Ui::MysqlInfoPopDlg* ui;
	SqlTableInfo m_stTableInfo;

	bool m_bUseDefaultIni;
	QString m_strIniFileName;
	QString m_strIniPrefix;
};

