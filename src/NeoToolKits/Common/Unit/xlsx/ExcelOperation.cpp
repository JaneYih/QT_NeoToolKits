#include "ExcelOperation.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

ExcelOperation::ExcelOperation(QObject *parent)
	: QObject(parent)
{

}

ExcelOperation::~ExcelOperation()
{

}

QStringList ExcelOperation::LoadExcelColumns(const QString& fileName)
{
	QStringList strOutList;
	QXlsx::Document xlsx(fileName);
	CellRange cellRange = OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		int rowCount = cellRange.rowCount();
		int columnCount = cellRange.columnCount();
		for (int col = 1; col <= columnCount; col++)
		{
			Cell* cell = xlsx.cellAt(1, col);
			if (cell != NULL)
			{
				QVariant var = cell->value();
				strOutList.push_back(var.toString());
			}
		}
	}
	return strOutList;
}


CellRange ExcelOperation::OpenExcelSheet(const QXlsx::Document& xlsx, int index)
{
	//if (xlsx.load())
	{
		QXlsx::Workbook* workBook = xlsx.workbook();
		if (workBook)
		{
			QXlsx::Worksheet* workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(index));
			if (workSheet)
			{
				return workSheet->dimension();
			}
		}
	}
	//else
	//{
	//	//	QMessageBox::critical(nullptr, "error", "excel open fail:" + fileName);
	//}
	return CellRange();
}

int ExcelOperation::LoadExcelRowCount(const QString& fileName)
{
	QXlsx::Document xlsx(fileName);
	CellRange cellRange = OpenExcelSheet(xlsx);
	if (cellRange.isValid())
	{
		return cellRange.rowCount();
	}
	return 0;
}
