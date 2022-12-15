#pragma once

#include <QStyledItemDelegate>
#include "ExcelDataUpload_def.h"

namespace NAMESPACENAME_EXCEL_DATA_UPLOAD
{
	class ExcelDataUploadDataDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		ExcelDataUploadDataDelegate(QObject* parent = nullptr);
		~ExcelDataUploadDataDelegate();

		// editing
		virtual QWidget* createEditor(QWidget* parent,
										const QStyleOptionViewItem& option,
										const QModelIndex& index) const override;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
		virtual void setModelData(QWidget* editor,
									QAbstractItemModel* model,
									const QModelIndex& index) const override;

		void setComboCtrlOptions(const QStringList& options);

	private:
		QStringList m_comboCtrlOptions;
	};
};

