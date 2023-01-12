#include "DbScriptEditor_def.h"
#include <QMetaEnum>

QDebug operator<<(QDebug dbg, const TestItem& item)
{
	dbg.nospace() << "----------------------\r\n";
	QMetaEnum metaEnum = QMetaEnum::fromType<TestItem::TestItemOperate>();
	QString text = QString("[%1][%2]").arg(QString(metaEnum.valueToKey(item.Operate()))).arg(item.toString());
	foreach(QString var, text.split(" ", QString::SkipEmptyParts))
	{
		dbg.nospace() << var << "\r\n";
	}
	dbg.nospace() << "----------------------";
	return dbg.maybeSpace();
}
