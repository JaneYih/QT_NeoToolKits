#pragma once

#include <QStyledItemDelegate>
#include "DatabaseDataExport_def.h"

namespace NAMESPACENAME_DATABASE_DATA_EXPORT
{
	class DatabaseDataExportDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		DatabaseDataExportDelegate(QObject* parent = nullptr);
		~DatabaseDataExportDelegate();

		//// painting
		void paint(QPainter* painter,
			const QStyleOptionViewItem& option, const QModelIndex& index) const override;
		QSize sizeHint(const QStyleOptionViewItem& option,
			const QModelIndex& index) const override;

		// editing
		virtual QWidget* createEditor(QWidget* parent,
										const QStyleOptionViewItem& option,
										const QModelIndex& index) const override;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
		virtual void setModelData(QWidget* editor,
									QAbstractItemModel* model,
									const QModelIndex& index) const override;
		/*void updateEditorGeometry(QWidget* editor,
			const QStyleOptionViewItem& option,
			const QModelIndex& index) const override;*/

		int GetDelegateColumnIndex() const;

	protected:
		bool editorEvent(QEvent* event, QAbstractItemModel* model,
			const QStyleOptionViewItem& option, const QModelIndex& index) override;
	};
};

