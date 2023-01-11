#include "DBScriptDataTableView.h"
#include <QMouseEvent>

DBScriptDataTableView::DBScriptDataTableView(QWidget *parent)
	: QTableView(parent)
{}

DBScriptDataTableView::~DBScriptDataTableView()
{}

void DBScriptDataTableView::mouseDoubleClickEvent(QMouseEvent* event)
{
	QTableView::mouseDoubleClickEvent(event);
}