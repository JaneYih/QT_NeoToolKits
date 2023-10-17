#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include "DatabaseDataExport_def.h"

namespace NAMESPACENAME_DATABASE_DATA_EXPORT
{
	class DatabaseDataExportModel : public QAbstractTableModel
	{
		Q_OBJECT

	public:
		DatabaseDataExportModel(QObject* parent = nullptr);
		~DatabaseDataExportModel();

	public:
		//void initData(const QStringList& exportDataColumns);
		void setData(const QVector<ExportDataUnit>& data);
		QVector<ExportDataUnit> getData() const;

	protected:
		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

	private:
		mutable QVector<ExportDataUnit> m_stData;
		void ClearData();
	};

};
