#include "DBScriptTestItemsModel.h"
#include <QColor>

DBScriptTestItemsModel::DBScriptTestItemsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	m_headerDatas.clear();
	m_headerDatas.push_back(QString::fromStdWString(L"ÐòºÅ"));
	m_headerDatas.push_back(QString::fromStdWString(L"²âÊÔÏî"));
}

DBScriptTestItemsModel::~DBScriptTestItemsModel()
{
}

int DBScriptTestItemsModel::rowCount(const QModelIndex& parent) const
{
	return m_testItems.count();
}

int DBScriptTestItemsModel::columnCount(const QModelIndex& parent) const
{
	return m_headerDatas.count();
}

QVariant DBScriptTestItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		if (Qt::Horizontal == orientation)
		{
			if (m_headerDatas.count() > section)
			{
				return m_headerDatas[section];
			}
		}
		else
		{
			return QVariant(QString("%1").arg(section + 1));
		}
	}
	return QVariant();
}

QVariant DBScriptTestItemsModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid())
	{
		TestItem* testitem = nullptr;
		int row = index.row();
		int column = index.column();
		if (row < m_testItems.count())
		{
			testitem = const_cast<TestItem*>(&m_testItems[row]);
		}

		if (testitem)
		{
			if (role == Qt::DisplayRole
				|| role == Qt::ToolTipRole
				|| role == Qt::EditRole)
			{
				if (column == 0)
				{
					return QVariant(QString("%1").arg(row + 1));
				}
				else if (column == 1)
				{
					return QVariant(testitem->toString());
				}
			}
			else if (role == Qt::BackgroundRole)
			{
				if (testitem->isWaitingDelete())
				{
					return QColor(240, 178, 157);//ºì
				}
				else if (testitem->isWaitingInsert())
				{
					return QColor(180, 240, 156);//ÂÌ
				}
				else if (testitem->isWaitingUpdate())
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

bool DBScriptTestItemsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid())
	{
		if (role == Qt::EditRole && 1 == index.column())
		{
			int row = index.row();
			if (row < m_testItems.count())
			{
				TestItem* testitem = const_cast<TestItem*>(&m_testItems[row]);
				if (testitem)
				{
					testitem->setValue(value.toString(), value.toString());
					if (!testitem->isWaitingOperate())
					{
						testitem->setWaitingUpdate();
					}
					emit dataChanged(index, index);
					return true;
				}
			}
		}
	}
	return false;
}

Qt::ItemFlags DBScriptTestItemsModel::flags(const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
	}
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
}

bool DBScriptTestItemsModel::removeRows(int row, int count, const QModelIndex& parent)
{
	int last = row + count - 1;
	beginRemoveRows(parent, row, last);
	for (int i = row; i <= last; ++i)
	{
		if (i < m_testItems.count())
		{
			m_testItems.removeAt(i);
		}
	}
	endRemoveRows();
	return true;
}

QList<TestItem> DBScriptTestItemsModel::getTestItems() const
{
	return m_testItems;
}

void DBScriptTestItemsModel::setTestItems(const QList<TestItem>& items)
{
	int rowCount = items.count() - 1;
	if (rowCount >= 0)
	{
		beginInsertRows(QModelIndex(), 0, rowCount);
		m_testItems = items;
		/*m_testItems.clear();
		for each (auto var in items)
		{
			m_testItems.push_back(var);
		}*/
		endInsertRows();
	}
}

void DBScriptTestItemsModel::ClearTestItems()
{
	int rowCount = m_testItems.count() - 1;
	if (rowCount >= 0)
	{
		beginRemoveRows(QModelIndex(), 0, rowCount);
		m_testItems.clear();
		endRemoveRows();
	}
}
bool DBScriptTestItemsModel::setTestItem(const QModelIndex& index, const TestItem& strValue)
{
	return setData(index, strValue.toString(), Qt::EditRole);
}

TestItem DBScriptTestItemsModel::getTestItem(const QModelIndex& index) const
{
	QString value = data(index, Qt::EditRole).toString();
	return TestItem(value, value);
}

void DBScriptTestItemsModel::insertRow(const QModelIndex& selection)
{
	int row = m_testItems.count() - 1;
	row = row < 0 ? 0 : row;
	if (selection.isValid())
	{
		row = selection.row();
	}

	if (row > m_testItems.count())
	{
		return;
	}

	beginInsertRows(QModelIndex(), row, row);
	TestItem newTestItem("", "");
	newTestItem.setWaitingInsert();
	m_testItems.insert(row + 1, newTestItem);
	endInsertRows();
}

bool DBScriptTestItemsModel::removeRows(const QModelIndexList& selection)
{
	for (int j = selection.size() - 1; j >= 0; --j)
	{
		QModelIndex curModelIndex = selection.at(j);
		if (curModelIndex.isValid())
		{
			int curRow = curModelIndex.row();
			if (curRow < m_testItems.count())
			{
				TestItem* testitem = const_cast<TestItem*>(&m_testItems[curRow]);
				testitem->setWaitingDelete();
			}
		}
	}

	for (int i = 0; i < m_testItems.count();)
	{
		if (m_testItems[i].isWaitingDelete())
		{
			removeRows(i, 1);
			i = 0;
		}
		else
		{
			i++;
		}
	}
	return true;
}
