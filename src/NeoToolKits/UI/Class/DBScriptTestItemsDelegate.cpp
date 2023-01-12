#include "DBScriptTestItemsDelegate.h"
#include "DBScriptTestItemsModel.h"
#include <QComboBox>
#include <QCompleter>

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DBScriptTestItemsDelegate::DBScriptTestItemsDelegate(QMap<QString, QString>* hTestItemDictionary, QObject *parent)
	: QStyledItemDelegate(parent),
	m_hTestItemDictionary(hTestItemDictionary)
{
	Q_ASSERT(m_hTestItemDictionary);
	m_comboCtrlOptions = getTestItemsByKeyword("");
}

DBScriptTestItemsDelegate::~DBScriptTestItemsDelegate()
{}

QStringList DBScriptTestItemsDelegate::getTestItemsByKeyword(const QString& keyword) const
{
	QStringList outTestItems;
	QMapIterator<QString, QString> i(*m_hTestItemDictionary);
	while (i.hasNext())
	{
		i.next();
		TestItem testItem(i.key(), i.value());
		QString strTestItem = testItem.toString();
		if (strTestItem.indexOf(keyword) != -1)
		{
			outTestItems.push_back(strTestItem);
		}
	}
	return outTestItems;
}

void DBScriptTestItemsDelegate::setComboCtrlOptionsByKeyword(QComboBox* comboControl, const QString& keyword) const
{
	if (comboControl)
	{
		comboControl->clear();
		QStringList testItems = getTestItemsByKeyword(keyword);
		comboControl->addItems(testItems);
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
		QStringList testItems = getTestItemsByKeyword(strData);
		if (testItems.count() == 1)
		{
			if (testItems[0] == strData)
			{
				TestItem testItem;
				testItem.fromString(strData);
				model->setData(index, QVariant::fromValue<TestItem>(testItem));
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
