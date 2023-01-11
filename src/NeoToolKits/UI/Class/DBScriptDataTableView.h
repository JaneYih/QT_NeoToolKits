#pragma once

#include <QTableView>

class DBScriptDataTableView  : public QTableView
{
	Q_OBJECT

public:
	DBScriptDataTableView(QWidget *parent);
	~DBScriptDataTableView();

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
};
