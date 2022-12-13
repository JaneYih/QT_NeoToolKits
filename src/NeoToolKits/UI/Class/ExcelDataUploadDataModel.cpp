#include "ExcelDataUploadDataModel.h"
#include <QMessageBox>

ExcelDataUploadDataModel::ExcelDataUploadDataModel(QObject* parent)
	: QAbstractTableModel(parent)
{

}

ExcelDataUploadDataModel::~ExcelDataUploadDataModel()
{

}

void ExcelDataUploadDataModel::initData(const QStringList& excelColumns)
{
	QVector<ExcelDataUploadInfo> data;
	int index = 0;
	foreach (QString var, excelColumns)
	{
		data.append(ExcelDataUploadInfo(++index, var));
	}
	setData(data);
}

void ExcelDataUploadDataModel::setData(const QVector<ExcelDataUploadInfo>& data)
{
	ClearData();
	int count = data.count() - 1;
	if (count >= 0)
	{
		beginInsertRows(QModelIndex(), 0, count);
		m_stData = data;
		endInsertRows();
	}
}

QVector<ExcelDataUploadInfo> ExcelDataUploadDataModel::getData() const
{
	return m_stData;
}

void ExcelDataUploadDataModel::ClearData()
{
	int count = m_stData.count() - 1;
	if (count >= 0)
	{
		beginRemoveRows(QModelIndex(), 0, count);
		m_stData.clear();
		endRemoveRows();
	}
}

QModelIndex ExcelDataUploadDataModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row >= 0 && column >= 0 && row < m_stData.count())
	{
		return createIndex(row, column, &m_stData[row]);
	}
	return QModelIndex();
}

Qt::ItemFlags ExcelDataUploadDataModel::flags(const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ExcelDataUploadDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
	{
		if (Qt::Horizontal == orientation)
		{
			switch (section)
			{
			case 0:
				return QVariant(QString::fromStdWString(L"Excel列名"));
			case 1:
				return QVariant(QString::fromStdWString(L"数据库字段"));
			default:
				break;
			}
		}
		else
		{
			return QVariant(QString("%1").arg(section + 1));
		}
	}
	return QVariant();
}

int ExcelDataUploadDataModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return m_stData.count();
}

int ExcelDataUploadDataModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return 2;
}

QVariant ExcelDataUploadDataModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid())
	{
		int row = index.row();
		int col = index.column();
		if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
		{
			if (m_stData.count() > row)
			{
				switch (col)
				{
				case 0:
					return m_stData[row].ExcelColumnName;
				case 1:
					return m_stData[row].DbFieldName;
				}
			}
		}
	}
	return QVariant();
}

bool ExcelDataUploadDataModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid()
		&& index.column() == 1
		&& role == Qt::EditRole)
	{
		int row = index.row();
		if (row < m_stData.count())
		{
			QString strValue = value.toString();
			for (int i = 0; i < m_stData.count(); ++i)
			{
				if (m_stData[i].DbFieldName == strValue
					&& i != row
					&& strValue != "")
				{
					QMessageBox::warning(nullptr, "warning", QString::fromStdWString(L"%1已经被其他项(%2)选择使用，禁止重复选择！").arg(strValue).arg(m_stData[i].ExcelColumnName));
					return false;
				}
			}

			m_stData[row].DbFieldName = strValue;
			return true;
		}
	}
	return false;
}
