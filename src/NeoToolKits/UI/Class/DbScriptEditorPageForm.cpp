#include "DbScriptEditorPageForm.h"
#include "Database_def.h"
#include "DataTableTest.h"
#include <QDebug>

DbScriptEditorPageForm::DbScriptEditorPageForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::DbScriptEditorPageForm)
{
	initView();

	SqlTableInfo tempInfo;
	tempInfo.baseInfo.type = SqlTypes::eSQLITE;
	tempInfo.baseInfo.dbName = "C:\\Users\\20200427027\\Desktop\\testitemcfg.db";
	CDataTableTest db(tempInfo.baseInfo);
	QString tableName("model_testlist");
	if (db.IsExistTable(tableName.toLocal8Bit()))
	{
		std::list<string> Fields;
		db.GetTableFullFields(tableName.toLocal8Bit(), Fields);
	}
}

DbScriptEditorPageForm::~DbScriptEditorPageForm()
{
	delete ui;
}

void DbScriptEditorPageForm::initView(void)
{
	ui->setupUi(this);
}
