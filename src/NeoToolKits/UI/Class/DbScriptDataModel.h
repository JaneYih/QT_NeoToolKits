#pragma once

#include <QAbstractTableModel>
#include "DbScriptEditor_def.h"

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DbScriptDataModel : public QAbstractTableModel
	{
		Q_OBJECT

	public:
		DbScriptDataModel(QObject* parent = nullptr);
		~DbScriptDataModel();

	public:


	protected:
		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
		virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

	};
}


