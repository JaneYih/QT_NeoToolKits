#pragma once

#include <QDialog>
#include "ui_MysqlInfoPopDialog.h"
#include "Database_def.h"
#include "IniOperation.h"

class MysqlInfoPopDialog  : public QDialog
{
	Q_OBJECT

public:
	MysqlInfoPopDialog(QWidget *parent = nullptr);
	~MysqlInfoPopDialog();

	QString infoString();

public slots:
	void PushbuttonClickedSlot(bool checked = false);

private:
	void initView(void);
	bool LoadCfg();
	bool SaveCfg();
	void LoadUiData();

private:
	Ui::MysqlInfoPopDlg* ui;
	SqlTableInfo m_stTableInfo;
	IniOperation* m_pCfg;
};

