#include "DatabaseDataExportModel.h"
#include <QMessageBox>

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

DatabaseDataExportModel::DatabaseDataExportModel(QObject* parent)
	: QAbstractTableModel(parent)
{

}

DatabaseDataExportModel::~DatabaseDataExportModel()
{

}

void DatabaseDataExportModel::initData(const QStringList& exportDataColumns)
{
	QVector<ExportDataUnit> data;
	int index = 0;
	foreach (QString var, exportDataColumns)
	{
		data.append(ExportDataUnit(var, var));
	}
	setData(data);
}

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
		//case 0:
		//	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
		case 2:
			return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		//default:
		//	break;
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
			case 0:
				return QVariant(QString::fromStdWString(L"是否导出"));
			case 1:
				return QVariant(QString::fromStdWString(L"数据库字段"));
			case 2:
				return QVariant(QString::fromStdWString(L"Excel标题"));
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
	return 3;
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
				case 0:
					return m_stData[row].bExport 
							? QString::fromStdWString(L"导出") 
							: QString::fromStdWString(L"不导出");
				case 1:
					return m_stData[row].DbKey;
				case 2:
					return m_stData[row].ExcelTitle;
				}
			}
			/*else if (role == Qt::CheckStateRole)
			{
				if (col == 0)
				{
					return m_stData[row].bExport;
				}
			}*/
			else if (role == Qt::BackgroundRole)
			{
				if (m_stData[row].bExport)
				{
					return QColor(243, 245, 152);//黄
				}
				return QColor(Qt::white);
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
			if (index.column() == 2)
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
			else if (index.column() == 0)
			{
				m_stData[row].bExport = value.toBool();
				emit dataChanged(index, createIndex(row, columnCount()));
				return true;
			}
		}
	}
	return false;
}
