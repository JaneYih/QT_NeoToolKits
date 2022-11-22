#include "DbScriptEditorPageForm.h"

DbScriptEditorPageForm::DbScriptEditorPageForm(QWidget *parent)
	: QWidget(parent), 
	ui(new Ui::DbScriptEditorPageForm)
{
	initView();
}

DbScriptEditorPageForm::~DbScriptEditorPageForm()
{
	delete ui;
}

void DbScriptEditorPageForm::initView(void)
{
	ui->setupUi(this);
}
