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

		void paint(QPainter* painter,
			const QStyleOptionViewItem& option, const QModelIndex& index) const override;

		QList<int> GetDelegateColumnIndexs() const;

	protected:
		bool editorEvent(QEvent* event, QAbstractItemModel* model,
			const QStyleOptionViewItem& option, const QModelIndex& index) override;

	private:
		QList<int> m_ColumnIndexs;

	private:
		void DatabaseDataExportDelegate::paintSub(QPainter* painter,
			const QStyleOptionViewItem& option,
			bool value,
			const QColor& checkedColor,
			const QColor& uncheckedColor) const;

	};
};

