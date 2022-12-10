#pragma once

#include <QAbstractTableModel>
#include <QVector>


typedef struct _ExcelDataUploadInfo_
{
	QString ExcelColumnName; //Excel数据列名称
	QString DbFieldName; //数据库字段名
	explicit _ExcelDataUploadInfo_(const QString& excelColumnName, const QString& dbFieldName = "")
		: ExcelColumnName(excelColumnName),
		DbFieldName(dbFieldName)
	{
	}
}ExcelDataUploadInfo, * pExcelDataUploadInfo;

class ExcelDataUploadDataModel  : public QAbstractTableModel
{
	Q_OBJECT

public:
	ExcelDataUploadDataModel(QObject *parent = nullptr);
	~ExcelDataUploadDataModel();

public:
	void initData(const QStringList& excelColumns);
	void setData(const QVector<ExcelDataUploadInfo>& data);
	QVector<ExcelDataUploadInfo> getData() const;

protected:
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
	mutable QVector<ExcelDataUploadInfo> m_stData;
	void ClearData();
};
