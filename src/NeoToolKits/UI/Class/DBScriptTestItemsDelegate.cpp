#include "DBScriptTestItemsDelegate.h"
#include "DBScriptTestItemsModel.h"
#include <QComboBox>
#include <QCompleter>

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DBScriptTestItemsDelegate::DBScriptTestItemsDelegate(QMap<QString, TestItem>* hTestItemDictionary, QObject *parent)
	: QStyledItemDelegate(parent),
	m_hTestItemDictionary(hTestItemDictionary)
{
	Q_ASSERT(m_hTestItemDictionary);
	m_comboCtrlOptions.clear();
	QList<TestItem> testItems = getTestItemsByKeyword("");
	for each (auto var in testItems)
	{
		m_comboCtrlOptions.append(var.toString());
	}
}

DBScriptTestItemsDelegate::~DBScriptTestItemsDelegate()
{}

QList<TestItem> DBScriptTestItemsDelegate::getTestItemsByKeyword(const QString& keyword) const
{
	QList<TestItem> outTestItems;
	QMapIterator<QString, TestItem> i(*m_hTestItemDictionary);
	while (i.hasNext())
	{
		i.next();
		QString strTestItem = i.value().toString();
		if (strTestItem.indexOf(keyword) != -1)
		{
			outTestItems.push_back(i.value());
		}
	}
	return outTestItems;
}

void DBScriptTestItemsDelegate::setComboCtrlOptionsByKeyword(QComboBox* comboControl, const QString& keyword) const
{
	if (comboControl)
	{
		comboControl->clear();
		QList<TestItem> testItems = getTestItemsByKeyword(keyword);
		for each (auto var in testItems)
		{
			comboControl->addItem(var.toString());
		}
	}
}

QWidget* DBScriptTestItemsDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		QComboBox* comboControl = new QComboBox(parent);
		comboControl->setEditable(true);
		comboControl->clear();
		comboControl->addItems(m_comboCtrlOptions);
		QCompleter* completer = new QCompleter(m_comboCtrlOptions, parent);
		completer->setFilterMode(Qt::MatchContains);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		comboControl->setCompleter(completer);
		return comboControl;
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void DBScriptTestItemsDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		QComboBox* comboControl = static_cast<QComboBox*>(editor);
		TestItem* pTestItem = static_cast<TestItem*>(index.internalPointer());
		if (comboControl && pTestItem)
		{
			comboControl->setCurrentText(pTestItem->toString());
		}
	}
	QStyledItemDelegate::setEditorData(editor, index);
}

void DBScriptTestItemsDelegate::setModelData(QWidget* editor, 
	QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.isValid() && index.column() == 1)
	{
		QComboBox* comboControl = static_cast<QComboBox*>(editor);
		QString strData = comboControl->currentText();
		QList<TestItem> testItems = getTestItemsByKeyword(strData);
		if (testItems.count() == 1)
		{
			if (testItems[0].toString() == strData)
			{
				model->setData(index, QVariant::fromValue<TestItem>(testItems[0]));
			}
		}
		return;
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void DBScriptTestItemsDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}
