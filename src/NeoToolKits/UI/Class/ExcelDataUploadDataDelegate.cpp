#include "ExcelDataUploadDataDelegate.h"
#include "ExcelDataUploadDataModel.h"
#include <QComboBox>

using namespace ExcelDataUpload;

ExcelDataUploadDataDelegate::ExcelDataUploadDataDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

ExcelDataUploadDataDelegate::~ExcelDataUploadDataDelegate()
{}

void ExcelDataUploadDataDelegate::setComboCtrlOptions(const QStringList& options)
{
	m_comboCtrlOptions = options;
}

QWidget* ExcelDataUploadDataDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		QComboBox* comboControl = new QComboBox(parent);
		comboControl->addItems(m_comboCtrlOptions);
		comboControl->setEditable(false);
		return comboControl;
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void ExcelDataUploadDataDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		ExcelDataUploadInfo* pData = static_cast<ExcelDataUploadInfo*>(index.internalPointer());
		if (pData)
		{
			QComboBox* comboControl = static_cast<QComboBox*>(editor);
			comboControl->setCurrentText(pData->DbFieldName);
			return;
		}
	}
	QStyledItemDelegate::setEditorData(editor, index);
}

void ExcelDataUploadDataDelegate::setModelData(QWidget* editor, 
	QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		QComboBox* comboControl = static_cast<QComboBox*>(editor);
		model->setData(index, comboControl->currentText());
		return;
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}
