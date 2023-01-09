#include "DbScriptDataModel.h"

DbScriptDataModel::DbScriptDataModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

DbScriptDataModel::~DbScriptDataModel()
{

}

DbData DbScriptDataModel::getDbScriptData() const
{
	return m_DbScriptData;
}

void DbScriptDataModel::setDbScriptData(const DbData& data)
{
	int rowCount = data.rows.count() - 1;
	int columnCount = data.fieldGroup.fields.size() - 1;
	if (rowCount >= 0 && columnCount >= 0)
	{
		beginInsertRows(QModelIndex(), 0, rowCount);
		beginInsertColumns(QModelIndex(), 0, columnCount);
		m_DbScriptData = data;
		endInsertColumns();
		endInsertRows();
	}
}

QModelIndex DbScriptDataModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row >= 0 && column >= 0 && row < m_DbScriptData.rows.size())
	{
		pDbFieldGroup pRowData = m_DbScriptData.rows[row];
		if (pRowData != nullptr
			&& column < pRowData->fields.size())
		{
			return createIndex(row, column, &m_DbScriptData.rows[row]->fields[column]);
		}
	}
	return QModelIndex();
}

int DbScriptDataModel::rowCount(const QModelIndex& parent) const
{
	return m_DbScriptData.rows.size();
}

int DbScriptDataModel::columnCount(const QModelIndex& parent) const
{
	return m_DbScriptData.fieldGroup.fields.size();
}

QVariant DbScriptDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		if (Qt::Horizontal == orientation)
		{
			if (section < m_DbScriptData.fieldGroup.fields.size())
			{
				return QVariant(QString::fromStdWString(L"%1").arg(m_DbScriptData.fieldGroup.fields[section]));
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
	if (index.isValid())
	{
		if (role == Qt::DisplayRole
			|| role == Qt::ToolTipRole
			|| role == Qt::EditRole)
		{
			int row = index.row();
			if (row < m_DbScriptData.rows.size())
			{
				pDbFieldGroup pRowData = m_DbScriptData.rows[row];
				int column = index.column();
				if (pRowData != nullptr
					&& column < pRowData->fields.size())
				{
					return pRowData->fields[column];
				}
			}
		}
	}
	return QVariant();
}

bool DbScriptDataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid())
	{
		if (role == Qt::EditRole)
		{
			int row = index.row();
			if (row < m_DbScriptData.rows.size())
			{
				pDbFieldGroup pRowData = m_DbScriptData.rows[row];
				int column = index.column();
				if (pRowData != nullptr
					&& column < pRowData->fields.size())
				{
					pRowData->fields[column] = value.toString();
					return true;
				}
			}
		}
	}
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
