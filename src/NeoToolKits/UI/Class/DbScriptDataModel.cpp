#include "DbScriptDataModel.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DbScriptDataModel::DbScriptDataModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

DbScriptDataModel::~DbScriptDataModel()
{

}

QModelIndex DbScriptDataModel::index(int row, int column, const QModelIndex& parent) const
{
	return QModelIndex();
}

int DbScriptDataModel::rowCount(const QModelIndex& parent) const
{
	return 2;
}

int DbScriptDataModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QVariant DbScriptDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		if (Qt::Horizontal == orientation)
		{
			switch (section)
			{
			case 0:
				return QVariant(QString::fromStdWString(L"Excel列名"));
			case 1:
				return QVariant(QString::fromStdWString(L"数据库字段"));
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

QVariant DbScriptDataModel::data(const QModelIndex& index, int role) const
{
	return QVariant();
}

bool DbScriptDataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	return false;
}

Qt::ItemFlags DbScriptDataModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void DbScriptDataModel::sort(int column, Qt::SortOrder order)
{
	return;
}
