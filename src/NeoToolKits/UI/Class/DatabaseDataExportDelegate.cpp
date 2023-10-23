#include "DatabaseDataExportDelegate.h"
#include "ExcelDataUploadDataModel.h"
#include <QCheckBox>
#include "iconhelper.h"

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;


/*��->�Ƿ񵼳�*/
const int c_isExport_col_index = 0;
/*��->���в�����*/
const int c_isEmptyUnexport_col_index = 1;

QList<int> DatabaseDataExportDelegate::GetDelegateColumnIndexs() const 
{
	return m_ColumnIndexs;
}

DatabaseDataExportDelegate::DatabaseDataExportDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_ColumnIndexs.push_back(c_isExport_col_index); ///*��->�Ƿ񵼳�*/
	m_ColumnIndexs.push_back(c_isEmptyUnexport_col_index); ///*��->���в�����*/
}

DatabaseDataExportDelegate::~DatabaseDataExportDelegate()
{}

void DatabaseDataExportDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid() && m_ColumnIndexs.contains(index.column()))
	{
		ExportDataUnit* pData = static_cast<ExportDataUnit*>(index.internalPointer());
		if (index.column() == c_isExport_col_index)
		{
			paintSub(painter,
				option,
				pData->bExport,
				pData->bAllEmptyUnexport 
					? QColor(247, 193, 236)  //��ɫ
					: QColor(243, 245, 152), //��ɫ
				QColor(Qt::white));
		}
		else if (index.column() == c_isEmptyUnexport_col_index)
		{
			paintSub(painter,
				option,
				pData->bAllEmptyUnexport,
				pData->bExport 
					? QColor(247, 193, 236) //��ɫ
					: QColor(Qt::white),
				pData->bExport 
					? QColor(243, 245, 152) //��ɫ
					: QColor(Qt::white)
			);
		}
		return;
	}
	return QStyledItemDelegate::paint(painter, option, index);
}

void DatabaseDataExportDelegate::paintSub(QPainter* painter, 
	const QStyleOptionViewItem& option, 
	bool value, 
	const QColor& checkedColor,
	const QColor& uncheckedColor) const
{
	QPen pen;
	pen.setBrush(Qt::transparent);
	painter->setPen(pen);
	QBrush barBackgroundBrush;
	barBackgroundBrush.setStyle(Qt::SolidPattern);
	barBackgroundBrush.setColor(value ? checkedColor : uncheckedColor);
	painter->setBrush(barBackgroundBrush);
	painter->drawRect(option.rect);

	QFont font = IconHelper::Instance()->GetIconFont();
	font.setPointSize(option.font.pointSize() * 2.5);
	painter->setFont(font);
	painter->setPen(QColor("#00aaff"));
	painter->setBrush(option.backgroundBrush);
	painter->drawText(option.rect, Qt::AlignCenter, value ? QChar(0xf205) : QChar(0xf204));
}


bool DatabaseDataExportDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
	const QStyleOptionViewItem& option, const QModelIndex& index)
{
	if (index.isValid() && m_ColumnIndexs.contains(index.column()))
	{
		ExportDataUnit* pData = static_cast<ExportDataUnit*>(index.internalPointer());
		QEvent::Type eEventType = event->type();
		qDebug() << index  << ": " << eEventType << endl;

		if (index.column() == c_isExport_col_index)
		{
			static QModelIndex isExport_lastIndex;
			if (eEventType == QEvent::MouseButtonPress
				|| (eEventType == QEvent::MouseMove && isExport_lastIndex != index))
			{
				if (eEventType == QEvent::MouseMove && isExport_lastIndex.isValid())//�����������ϻ�����
				{
					model->setData(isExport_lastIndex, !pData->bExport);
				}
				isExport_lastIndex = index;
				model->setData(index, !pData->bExport);
				return true;
			}
		}
		else if (index.column() == c_isEmptyUnexport_col_index)
		{
			static QModelIndex isEmptyUnexport_lastIndex;
			if (eEventType == QEvent::MouseButtonPress
				|| (eEventType == QEvent::MouseMove && isEmptyUnexport_lastIndex != index))
			{
				if (eEventType == QEvent::MouseMove && isEmptyUnexport_lastIndex.isValid())//�����������ϻ�����
				{
					model->setData(isEmptyUnexport_lastIndex, !pData->bAllEmptyUnexport);
				}
				isEmptyUnexport_lastIndex = index;
				model->setData(index, !pData->bAllEmptyUnexport);
				return true;
			}
		}
		
	}
	return QStyledItemDelegate::editorEvent(event, model, option, index);
}