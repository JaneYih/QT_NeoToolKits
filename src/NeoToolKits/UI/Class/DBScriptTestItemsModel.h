#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include "DbScriptEditor_def.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

class DBScriptTestItemsModel  : public QAbstractTableModel
{
	Q_OBJECT

public:
	DBScriptTestItemsModel(QObject *parent = nullptr);
	~DBScriptTestItemsModel();

public:
	QList<TestItem> getTestItems() const;
	void resetTestItems(const QList<TestItem>& items);
	void ClearTestItems();

	bool setTestItem(const QModelIndex& index, const TestItem& strValue);
	TestItem getTestItem(const QModelIndex& index) const;

	void insertRow(const QModelIndex& selection);
	bool removeRows(const QModelIndexList& selection);
	void removeWaitingOperateRows(TestItem::TestItemOperate operate);

protected:
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
	
	//virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count,
		const QModelIndex& destinationParent, int destinationChild) override;

	virtual QStringList mimeTypes() const override;
	virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
	virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action,
		int row, int column, const QModelIndex& parent) const override;
	virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action,
		int row, int column, const QModelIndex& parent) override;
	virtual Qt::DropActions supportedDropActions() const override;
	virtual Qt::DropActions supportedDragActions() const override;

private:
	QString ModelIndexsToText(const QModelIndexList& indexes) const;
	bool DropPackage(const QList<StringGroupItem>& sourceItems, int row, const QModelIndex& parent);

signals:
	void testItemsRowsMoved();

private:
	QList<TestItem> m_testItems;
	QVector<QString> m_headerDatas;
};
