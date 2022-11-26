#pragma once

#include <QWidget>
#include "ui_DbScriptEditorPageForm.h"

class DbScriptEditorPageForm  : public QWidget
{
	Q_OBJECT

public:
	DbScriptEditorPageForm(QWidget *parent = nullptr);
	~DbScriptEditorPageForm();

private:
	Ui::DbScriptEditorPageForm* ui;
	void initView(void);
};
