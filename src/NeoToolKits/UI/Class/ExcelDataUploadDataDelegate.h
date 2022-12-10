#pragma once

#include <QStyledItemDelegate>

class ExcelDataUploadDataDelegate  : public QStyledItemDelegate
{
	Q_OBJECT

public:
	ExcelDataUploadDataDelegate(QObject* parent = nullptr);
	~ExcelDataUploadDataDelegate();

	// editing
	QWidget* createEditor(QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const override;

	void setComboCtrlOptions(const QStringList& options);

private:
	QStringList m_comboCtrlOptions;
};
