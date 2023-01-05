#pragma once

#include <QObject>
#include "xlsxdocument.h"

class ExcelOperation  : public QObject
{
	Q_OBJECT

public:
	ExcelOperation(QObject *parent = nullptr);
	~ExcelOperation();

	static QStringList LoadExcelColumns(const QString& fileName);
	static int LoadExcelRowCount(const QString& fileName);
	static QXlsx::CellRange OpenExcelSheet(const QXlsx::Document& xlsx, int index = 0);
};
