#include "DBScriptTestItemsModel.h"
#include <QColor>
#include <QMimeData>

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

QModelIndex DBScriptTestItemsModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row >= 0 && column >= 0 && row < m_testItems.count())
	{
		return createIndex(row, column, (void*)(&m_testItems[row]));
	}
	return QModelIndex();
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
					if (testitem->isValid())
					{
						return QVariant(testitem->toString());
					}
					return QVariant();
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
					bool bChanged = false;
					if (value.canConvert<QString>())
					{
						QString inputValue(value.toString());
						if (testitem->toString() != inputValue)
						{
							testitem->fromString(inputValue);
							bChanged = true;
						}
					}
					else if (value.canConvert<TestItem>())
					{
						TestItem inputValue(value.value<TestItem>());
						if (*testitem != inputValue)
						{
							testitem->setValue(inputValue.code(), inputValue.name());
							bChanged = true;
						}
					}

					if (bChanged)
					{
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
	}
	return false;
}

Qt::ItemFlags DBScriptTestItemsModel::flags(const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	}
	return Qt::ItemIsSelectable  | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QStringList DBScriptTestItemsModel::mimeTypes() const
{
	QStringList types;
	types << "text/plain";
	return types;
}

QMimeData* DBScriptTestItemsModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* mineData = new QMimeData();
	mineData->setText(ModelIndexsToText(indexes));
	return mineData;
}

QString DBScriptTestItemsModel::ModelIndexsToText(const QModelIndexList& indexes) const
{
	QList<StringGroupItem> sourceItems;
	foreach(QModelIndex var, indexes)
	{
		int row = var.row();
		if (var.column() == 0 && row < m_testItems.count())
		{
			StringGroupItem stringGroupItem;
			stringGroupItem.index = row;
			stringGroupItem.text = m_testItems[row].toString();
			sourceItems.push_back(stringGroupItem);
		}
	}
	qSort(sourceItems.begin(), sourceItems.end(), StringGroupItem::compareIndexLessThan);

	QString strText;
	for each (auto var in sourceItems)
	{
		strText += QString("[%1]%2;").arg(var.index).arg(var.text);
	}
	return strText;
}

bool DBScriptTestItemsModel::canDropMimeData(const QMimeData* data, Qt::DropAction action,
	int row, int column, const QModelIndex& parent) const
{
	if (action == Qt::MoveAction
		&& data->hasText())
	{
		return true;
	}
	return false;
}

bool DBScriptTestItemsModel::dropMimeData(const QMimeData* data, Qt::DropAction action,
	int row, int column, const QModelIndex& parent)
{
	if (!canDropMimeData(data, action, row, column, parent))
	{
		return false;
	}

	if (action == Qt::IgnoreAction)
	{
		return true;
	}

	if (action == Qt::MoveAction)
	{
		QString strDataText(data->text());
		QStringList strDatas = strDataText.split(';');
		QList<StringGroupItem> sourceItems;
		for each (auto var in strDatas)
		{
			int firstIndex = var.indexOf('[') + 1;
			int lastIndex = var.indexOf(']');
			if (firstIndex <= 0 || lastIndex <= 1 || firstIndex > lastIndex)
			{
				continue;
			}
			StringGroupItem item;
			item.index = var.mid(firstIndex, lastIndex - firstIndex).toInt();
			item.text = var.mid(lastIndex+1);
			sourceItems.push_back(item);
		}
		return DropPackage(sourceItems, parent.row()+1, parent);
	}

	return false;
}

bool DBScriptTestItemsModel::DropPackage(const QList<StringGroupItem>& sourceItems, int row, const QModelIndex& parent)
{
	bool bDoneUpder = false;
	int upderItemCount = 0;
	for (int i = 0; i < sourceItems.count(); ++i)
	{
		int sourceRow = sourceItems[i].index;
		int testItemsCount = m_testItems.count();
		if (testItemsCount <= sourceRow)
		{
			return false;
		}
		if (sourceRow < row)  //ÉÏ¶Î
		{
			if (!moveRows(QModelIndex(), sourceRow - i, 1, QModelIndex(), row))
			{
				return false;
			}
			upderItemCount++;
			bDoneUpder = true;
		}
		else if (sourceRow > row) //ÏÂ¶Î
		{
			int offset = bDoneUpder ? i - upderItemCount : i;
			if (!moveRows(QModelIndex(), sourceRow, 1, QModelIndex(), row + offset))
			{
				return false;
			}
		}
		else
		{
			if (!m_testItems[sourceRow].isWaitingOperate())
			{
				m_testItems[sourceRow].setWaitingUpdate();
			}
		}
	}
	emit testItemsRowsMoved();
	return true;
}

bool DBScriptTestItemsModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count,
	const QModelIndex& destinationParent, int destinationChild)
{
	int sourceLast = sourceRow + count - 1;

	if (sourceRow < 0 || sourceLast < 0 || destinationChild < 0)
	{
		return false;
	}

	QList<TestItem> sourceTestItems;
	for (int i = 0; i < count; ++i)
	{
		int index = sourceRow + i;
		if (index < m_testItems.count())
		{
			sourceTestItems.push_back(m_testItems[index]);
			m_testItems[index].setWaitingMove();
		}
	}

	beginInsertRows(destinationParent, destinationChild, destinationChild + sourceTestItems.count() - 1);
	int index = 0;
	for each (auto var in sourceTestItems)
	{
		if (!var.isWaitingOperate())
		{
			var.setWaitingUpdate();
		}
		m_testItems.insert(destinationChild + index, var);
		++index;
	}
	endInsertRows();

	removeWaitingOperateRows(TestItem::TestItemOperate::TestItem_Move);
	return true;
}

Qt::DropActions DBScriptTestItemsModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

Qt::DropActions DBScriptTestItemsModel::supportedDragActions() const
{
	return Qt::MoveAction;
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

void DBScriptTestItemsModel::resetTestItems(const QList<TestItem>& items)
{
	ClearTestItems();
	int rowCount = items.count() - 1;
	if (rowCount >= 0)
	{
		beginInsertRows(QModelIndex(), 0, rowCount);
		m_testItems = items;
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
	return setData(index, QVariant::fromValue(strValue), Qt::EditRole);
}

TestItem DBScriptTestItemsModel::getTestItem(const QModelIndex& index) const
{
	TestItem item;
	item.fromString(data(index, Qt::EditRole).toString());
	return item;
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
				if (!testitem->isWaitingInsert())
				{
					testitem->setWaitingDelete();
				}
			}
		}
	}

	removeWaitingOperateRows(TestItem::TestItemOperate::TestItem_Insert);
	return true;
}

void DBScriptTestItemsModel::removeWaitingOperateRows(TestItem::TestItemOperate operate)
{
	for (int i = 0; i < m_testItems.count();)
	{
		if (m_testItems[i].Operate() == operate)
		{
			removeRows(i, 1);
			i = 0;
		}
		else
		{
			i++;
		}
	}
}

