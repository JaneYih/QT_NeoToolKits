#include "DbScriptDataModel.h"
#include <QColor>
#include "DbScriptEditorApp.h"

const QString DbScriptDataModel::s_TestListHeaderName("TESTLIST");

DbScriptDataModel::DbScriptDataModel(QObject* parent)
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
		ClearDbScriptData();
		beginInsertRows(QModelIndex(), 0, rowCount);
		beginInsertColumns(QModelIndex(), 0, columnCount);
		m_DbScriptData = data;
		endInsertColumns();
		endInsertRows();
	}
}

void DbScriptDataModel::setTestItemDictionary(const QMap<QString, TestItem>& mapTestItemDictionary)
{
	m_mapTestItemDictionary = mapTestItemDictionary;
}

void DbScriptDataModel::ClearDbScriptData()
{
	int rowCount = m_DbScriptData.rows.count() - 1;
	int columnCount = m_DbScriptData.fieldGroup.fields.size() - 1;
	if (rowCount >= 0)
	{
		beginRemoveRows(QModelIndex(), 0, rowCount);
		m_DbScriptData.rows.clear();
		endRemoveRows();
	}
	if (columnCount >= 0)
	{
		beginRemoveColumns(QModelIndex(), 0, columnCount);
		m_DbScriptData.fieldGroup.clear();
		endRemoveColumns();
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
	return m_DbScriptData.rows.count();
}

int DbScriptDataModel::columnCount(const QModelIndex& parent) const
{
	return m_DbScriptData.fieldGroup.fields.count();
}

QString DbScriptDataModel::GetHorizontalHeaderName(int section) const
{
	return headerData(section, Qt::Horizontal, Qt::DisplayRole).toString();
}

QVariant DbScriptDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		if (Qt::Horizontal == orientation)
		{
			if (section < m_DbScriptData.fieldGroup.fields.size())
			{
				return QVariant(QString::fromStdWString(L"%1").arg(m_DbScriptData.fieldGroup.fields[section].value()));
			}
		}
		else
		{
			return QVariant(QString("%1").arg(section + 1));
		}
	}
	return QVariant();
}

QString DbScriptDataModel::getItemData(const QModelIndex& index) const
{
	return data(index, Qt::EditRole).toString();
}

QVariant DbScriptDataModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid())
	{
		DbDataCell* cellValue = nullptr;
		pDbFieldGroup pRowData = nullptr;
		int row = index.row();
		if (row < m_DbScriptData.rows.size())
		{
			pRowData = m_DbScriptData.rows[row];
			int column = index.column();
			if (pRowData != nullptr
				&& column < pRowData->fields.size())
			{
				cellValue = &(pRowData->fields[column]);
			}
		}

		if (cellValue && pRowData)
		{
			if (role == Qt::DisplayRole
				|| role == Qt::ToolTipRole
				|| role == Qt::EditRole)
			{
				if (GetHorizontalHeaderName(index.column()) == s_TestListHeaderName
					&& role == Qt::ToolTipRole
					&& m_mapTestItemDictionary.size() > 0)
				{
					QList<TestItem> testItems = NAMESPACENAME_DB_SCRIPT_EDITOR::DbScriptEditorApp::TestItemsTextConverter(cellValue->value(), m_mapTestItemDictionary);
					QString strTipText("");
					strTipText += pRowData->fields[2].value() + "\r\n";
					strTipText += pRowData->fields[1].value() + "\r\n";
					int index = 0;
					for each (TestItem var in testItems)
					{
						strTipText += QString("%1. %2\r\n").arg(++index).arg(var.toString()); var.toString();
					}
					return strTipText;
				}
				return cellValue->value();
			}
			else if (role == Qt::BackgroundRole)
			{
				if (cellValue->isWaitingDelete())
				{
					return QColor(240, 178, 157);//ºì
				}
				else if (cellValue->isWaitingInsert())
				{
					return QColor(180, 240, 156);//ÂÌ
				}
				else if (cellValue->isWaitingUpdate())
				{
					return QColor(243, 245, 152);//»Æ
				}
				return QColor(Qt::white);
			}
			else if (role == Qt::ForegroundRole)
			{
				return QColor(Qt::black);
			}
		}
	}
	return QVariant();
}

bool DbScriptDataModel::setItemData(const QModelIndex& index, const QString& strValue)
{
	return setData(index, strValue, Qt::EditRole);
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
					DbDataCell* cell = &(pRowData->fields[column]);
					if (cell->value() != value)
					{
						cell->setValue(value.toString());
						if (!cell->isWaitingOperate())
						{
							cell->setWaitingUpdate();
						}
						emit dataChanged(index, index);
						return true;
					}
				}
			}
		}
	}
	return false;
}

Qt::ItemFlags DbScriptDataModel::flags(const QModelIndex& index) const
{
	if (GetHorizontalHeaderName(index.column()) == s_TestListHeaderName)
	{
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void DbScriptDataModel::sort(int column, Qt::SortOrder order)
{
	return;
}

bool DbScriptDataModel::removeRows(int row, int count, const QModelIndex& parent)
{
	int last = row + count - 1;
	beginRemoveRows(parent, row, last);
	for (int i = row; i <= last; ++i)
	{
		if (i < m_DbScriptData.rows.count())
		{
			m_DbScriptData.rows.removeAt(i);
		}
	}
	endRemoveRows();
	return true;
}

bool DbScriptDataModel::removeRows(const QModelIndexList& selection)
{
	int rowCount = m_DbScriptData.rows.count();
	for (int j = selection.size()-1; j >= 0; --j)
	{
		if (selection.at(j).isValid())
		{
			int curRow = selection.at(j).row();
			if (curRow < rowCount)
			{
				int fieldsCount = m_DbScriptData.rows[curRow]->fields.count();
				bool bRemoveRows = false;
				for (int i = 0; i < fieldsCount; ++i)
				{
					DbDataCell* cell = &(m_DbScriptData.rows[curRow]->fields[i]);
					if (cell->isWaitingInsert())
					{
						removeRows(curRow, 1);
						rowCount = m_DbScriptData.rows.count();
						bRemoveRows = true;
						break;
					}
					else
					{
						cell->setWaitingDelete();
					}
				}
				if (!bRemoveRows)
				{
					emit dataChanged(createIndex(curRow, 0, &(m_DbScriptData.rows[curRow]->fields[0])),
						createIndex(curRow, fieldsCount - 1, &(m_DbScriptData.rows[curRow]->fields[fieldsCount - 1])));
				}
			}
		}
	}
	return true;
}

void DbScriptDataModel::insertRow(const QModelIndex& selection)
{
	int row = m_DbScriptData.rows.count() - 1;
	if (selection.isValid())
	{
		row = selection.row();
	}

	if (m_DbScriptData.fieldGroup.fields.count() <= 0
		|| row >= m_DbScriptData.rows.count())
	{
		return;
	}

	beginInsertRows(QModelIndex(), row, row);
	pDbFieldGroup values = new DbFieldGroup();
	foreach (auto var, m_DbScriptData.fieldGroup.fields)
	{
		DbDataCell cell;
		cell.setValue("");
		cell.setWaitingInsert();
		values->fields.push_back(cell);
	}
	m_DbScriptData.rows.insert(row+1, values);
	endInsertRows();
}