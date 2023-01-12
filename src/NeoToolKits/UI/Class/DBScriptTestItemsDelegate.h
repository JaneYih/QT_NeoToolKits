#pragma once

#include <QStyledItemDelegate>
#include <QMap>
#include <QComboBox>
#include "DbScriptEditor_def.h"

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	class DBScriptTestItemsDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		explicit DBScriptTestItemsDelegate(QMap<QString, QString>* hTestItemDictionary, QObject* parent = nullptr);
		~DBScriptTestItemsDelegate();

		// editing
		virtual QWidget* createEditor(QWidget* parent,
										const QStyleOptionViewItem& option,
										const QModelIndex& index) const override;
		virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
		virtual void setModelData(QWidget* editor,
									QAbstractItemModel* model,
									const QModelIndex& index) const override;

		virtual void updateEditorGeometry(QWidget* editor,
			const QStyleOptionViewItem& option,
			const QModelIndex& index) const override;

	private:
		void setComboCtrlOptionsByKeyword(QComboBox* comboControl, const QString& keyword) const;
		QStringList getTestItemsByKeyword(const QString& keyword) const;

	private:
		QMap<QString, QString>* m_hTestItemDictionary;
		QStringList m_comboCtrlOptions;
	};
};

