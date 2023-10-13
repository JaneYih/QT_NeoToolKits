#include "DatabaseDataExportDelegate.h"
#include "ExcelDataUploadDataModel.h"
#include <QCheckBox>
#include "iconhelper.h"

using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

const int s_DelegateColumnIndex = 0;
int DatabaseDataExportDelegate::GetDelegateColumnIndex() const
{
	return s_DelegateColumnIndex;
}

DatabaseDataExportDelegate::DatabaseDataExportDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

DatabaseDataExportDelegate::~DatabaseDataExportDelegate()
{}

QWidget* DatabaseDataExportDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	//if (index.isValid() && index.column() == s_DelegateColumnIndex)
	//{
	//	QCheckBox* checkBoxControl = new QCheckBox(parent);
	//	checkBoxControl->setCheckable(true);
	//	checkBoxControl->setChecked(true);
	//	return checkBoxControl;
	//}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void DatabaseDataExportDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	//if (index.isValid() && index.column() == s_DelegateColumnIndex)
	//{
	//	ExportDataUnit* pData = static_cast<ExportDataUnit*>(index.internalPointer());
	//	if (pData)
	//	{
	//		QCheckBox* checkBoxControl = static_cast<QCheckBox*>(editor);
	//		checkBoxControl->setChecked(pData->bExport);
	//		return;
	//	}
	//}
	QStyledItemDelegate::setEditorData(editor, index);
}

void DatabaseDataExportDelegate::setModelData(QWidget* editor, 
	QAbstractItemModel* model, const QModelIndex& index) const
{
	//if (index.isValid() && index.column() == s_DelegateColumnIndex)
	//{
	//	QCheckBox* checkBoxControl = static_cast<QCheckBox*>(editor);
	//	model->setData(index, checkBoxControl->isChecked());
	//	return;
	//}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void DatabaseDataExportDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	Q_UNUSED(index)
	editor->setGeometry(option.rect);
	//QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

QSize DatabaseDataExportDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

void DatabaseDataExportDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid() && index.column() == s_DelegateColumnIndex)
	{
		ExportDataUnit* pData = static_cast<ExportDataUnit*>(index.internalPointer());
		painter->setPen(QColor("#00aaff"));
		QFont font = IconHelper::Instance()->GetIconFont();
		font.setPointSize(option.font.pointSize() * 2);
		painter->setFont(font);
		painter->setBrush(option.backgroundBrush);
		painter->drawText(option.rect, Qt::AlignLeft, pData->bExport ? QChar(0xf205) : QChar(0xf204));
		return;
	}
	return QStyledItemDelegate::paint(painter, option, index);
}

bool DatabaseDataExportDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
	const QStyleOptionViewItem& option, const QModelIndex& index)
{
	if (index.isValid() && index.column() == s_DelegateColumnIndex)
	{
		ExportDataUnit* pData = static_cast<ExportDataUnit*>(index.internalPointer());
		QEvent::Type eEventType = event->type();
		if (eEventType == QEvent::MouseButtonPress)
		{
			model->setData(index, !pData->bExport);
			return true;
		}
	}
	return QStyledItemDelegate::editorEvent(event, model, option, index);
}