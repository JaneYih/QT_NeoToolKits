#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include "ExcelDataUpload_def.h"

namespace NAMESPACENAME_EXCEL_DATA_UPLOAD
{
	class ExcelDataUploadDataModel : public QAbstractTableModel
	{
		Q_OBJECT

	public:
		ExcelDataUploadDataModel(QObject* parent = nullptr);
		~ExcelDataUploadDataModel();

	public:
		void initData(const QStringList& excelColumns);
		void setData(const QVector<ExcelDataUploadInfo>& data);
		QVector<ExcelDataUploadInfo> getData() const;

	protected:
		virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
		virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

	private:
		mutable QVector<ExcelDataUploadInfo> m_stData;
		void ClearData();
	};

};
