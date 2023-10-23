#include "DatabaseDataExportModel.h"
#include <QMessageBox>

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

const int c_colCount = 4; //列数
/*列->是否导出*/
const int c_isExport_col_index = 0;
const QString c_isExport_col_name = QString::fromStdWString(L"是否导出");
/*列->空列不导出*/
const int c_isEmptyUnexport_col_index = 1;
const QString c_isEmptyUnexport_col_name = QString::fromStdWString(L"空列不导出");
/*列->数据库字段*/
const int c_dbKey_col_index = 2;
const QString c_dbKey_col_name = QString::fromStdWString(L"数据库字段");
/*列->Excel标题*/
const int c_excelTitle_col_index = 3;
const QString c_excelTitle_col_name = QString::fromStdWString(L"Excel标题");

DatabaseDataExportModel::DatabaseDataExportModel(QObject* parent)
	: QAbstractTableModel(parent)
{

}

DatabaseDataExportModel::~DatabaseDataExportModel()
{

}

//void DatabaseDataExportModel::initData(const QStringList& exportDataColumns)
//{
//	QVector<ExportDataUnit> data;
//	int index = 0;
//	foreach (QString var, exportDataColumns)
//	{
//		data.append(ExportDataUnit(var, var));
//	}
//	setData(data);
//}

void DatabaseDataExportModel::setData(const QVector<ExportDataUnit>& data)
{
	ClearData();
	int count = data.count() - 1;
	if (count >= 0)
	{
		beginInsertRows(QModelIndex(), 0, count);
		m_stData = data;
		endInsertRows();
	}
}

QVector<ExportDataUnit> DatabaseDataExportModel::getData() const
{
	return m_stData;
}

void DatabaseDataExportModel::ClearData()
{
	int count = m_stData.count() - 1;
	if (count >= 0)
	{
		beginRemoveRows(QModelIndex(), 0, count);
		m_stData.clear();
		endRemoveRows();
	}
}

QModelIndex DatabaseDataExportModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row >= 0 && column >= 0 && row < m_stData.count())
	{
		return createIndex(row, column, &m_stData[row]);
	}
	return QModelIndex();
}

Qt::ItemFlags DatabaseDataExportModel::flags(const QModelIndex& index) const
{
	if (index.isValid())
	{
		switch (index.column())
		{
		case c_excelTitle_col_index:
			return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		}
	}
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DatabaseDataExportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		if (Qt::Horizontal == orientation)
		{
			switch (section)
			{
			case c_isExport_col_index:
				return QVariant(c_isExport_col_name);
			case c_isEmptyUnexport_col_index:
				return QVariant(c_isEmptyUnexport_col_name);
			case c_dbKey_col_index:
				return QVariant(c_dbKey_col_name);
			case c_excelTitle_col_index:
				return QVariant(c_excelTitle_col_name);
			default:
				break;
			}
		}
		else
		{
			return QVariant(QString("%1").arg(section + 1));
		}
	}
	return QVariant();
}

int DatabaseDataExportModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return m_stData.count();
}

int DatabaseDataExportModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return c_colCount;
}

QVariant DatabaseDataExportModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid())
	{
		int row = index.row();
		int col = index.column();
		if (m_stData.count() > row)
		{
			if (role == Qt::DisplayRole
				|| role == Qt::ToolTipRole
				|| role == Qt::EditRole)
			{
				switch (col)
				{
				case c_isExport_col_index:
					return m_stData[row].bExport 
							? QString::fromStdWString(L"导出") 
							: QString::fromStdWString(L"不导出");
				case c_isEmptyUnexport_col_index:
					return m_stData[row].bAllEmptyUnexport
						? QString::fromStdWString(L"空列不导出")
						: QString::fromStdWString(L"空列也导出");
				case c_dbKey_col_index:
					return m_stData[row].DbKey;
				case c_excelTitle_col_index:
					return m_stData[row].ExcelTitle;
				}
			}
			/*else if (role == Qt::CheckStateRole)
			{
				if (col == c_isExport_col_index)
				{
					return m_stData[row].bExport;
				}
			}*/
			else if (role == Qt::BackgroundRole)
			{
				if (m_stData[row].bExport && !m_stData[row].bAllEmptyUnexport)
				{
					return QColor(243, 245, 152);//黄
				}
				else if (m_stData[row].bExport && m_stData[row].bAllEmptyUnexport)
				{
					return QColor(247, 193, 236);//粉
				}
				else
				{
					return QColor(Qt::white);
				}
			}
		}
	}
	return QVariant();
}

bool DatabaseDataExportModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid()
		&& role == Qt::EditRole)
	{
		int row = index.row();
		if (row < m_stData.count())
		{
			if (index.column() == c_excelTitle_col_index)
			{
				QString strValue = value.toString();
				for (int i = 0; i < m_stData.count(); ++i)
				{
					if (m_stData[i].ExcelTitle == strValue
						&& i != row
						&& strValue != "")
					{
						QMessageBox::warning(nullptr, "warning", QString::fromStdWString(L"%1已经被其他项(%2)使用，禁止Excel标题重复！").arg(strValue).arg(m_stData[i].DbKey));
						return false;
					}
				}
				m_stData[row].ExcelTitle = strValue;
				return true;
			}
			else if (index.column() == c_isExport_col_index)
			{
				m_stData[row].bExport = value.toBool();
				emit dataChanged(index, createIndex(row, columnCount()));
				return true;
			}
			else if (index.column() == c_isEmptyUnexport_col_index)
			{
				m_stData[row].bAllEmptyUnexport = value.toBool();
				emit dataChanged(index, createIndex(row, columnCount()));
				return true;
			}
		}
	}
	return false;
}
