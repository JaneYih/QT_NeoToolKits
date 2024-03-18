#pragma once

#include <QAbstractTableModel>
#include "DbScriptEditor_def.h"
#include "Database_def.h"

class DbScriptDataModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	DbScriptDataModel(QObject* parent = nullptr);
	~DbScriptDataModel();

public:
	DbData getDbScriptData() const;
	void setDbScriptData(const DbData& data);
	void setTestItemDictionary(const QMap<QString, TestItem>& mapTestItemDictionary);
	void ClearDbScriptData();
	bool setItemData(const QModelIndex& index, const QString& strValue);
	QString getItemData(const QModelIndex& index) const;
	void insertRow(const QModelIndex& selection);
	bool removeRows(const QModelIndexList& selection);
	QString GetHorizontalHeaderName(int section) const;

protected:
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
	//virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

private:
	mutable DbData m_DbScriptData;
	QMap<QString, TestItem> m_mapTestItemDictionary;

public:
	static const QString s_TestListHeaderName;
	static const QString s_IDHeaderName;
};


