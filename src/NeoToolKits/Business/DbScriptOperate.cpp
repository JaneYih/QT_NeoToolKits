#include "DbScriptOperate.h"

using namespace NAMESPACENAME_DB_SCRIPT_EDITOR;

DbScriptOperate::DbScriptOperate(const SqlTableInfo& info)
	: ITableManage(info.baseInfo),
	m_stSqlInfo(info)
{
	m_stSqlInfo.tableName = "model_testlist";
}

DbScriptOperate::~DbScriptOperate()
{

}

bool DbScriptOperate::TestConnect()
{
	DbFieldGroup Fields;
	QString strErrorMsg;
	return GetTableFullFields(Fields, strErrorMsg);
}

bool DbScriptOperate::CheckConnect(QString& strErrorMsg)
{
	if (!DatabaseInstence->IsInit())
	{
		strErrorMsg = QString::fromStdWString(L"数据库未连接");
		return false;
	}
	return true;
}

bool DbScriptOperate::GetTableFullFields(DbFieldGroup& outFields, QString& strErrorMsg)
{
	if (!CheckConnect(strErrorMsg))
	{
		return false;
	}

	outFields.clear();

	QByteArray byteTableName = m_stSqlInfo.tableName.toLocal8Bit();
	if (DatabaseInstence->IsExistTable(byteTableName))
	{
		FieldList Fields;
		DatabaseInstence->GetTableFullFields(byteTableName, Fields);
		outFields = Fields;
		return outFields.fields.size() > 0;
	}
	strErrorMsg = QString::fromStdWString(L"数据库表(%1)不存在").arg(m_stSqlInfo.tableName);
	return false;
}

bool DbScriptOperate::GetTableFullData(DbData& outData, QString& strErrorMsg,
	const QString& orderByField, Qt::SortOrder order)
{
	if (!CheckConnect(strErrorMsg))
	{
		return false;
	}

	outData.clear();

	DataTable ResultData;
	QString strOrderBy("");
	if (!orderByField.isEmpty())
	{
		QString strSortOrder;
		if (order == Qt::SortOrder::DescendingOrder)
		{
			strSortOrder = "desc";
		}
		else
		{
			strSortOrder = "asc";
		}
		strOrderBy = QString("order by %1 %2").arg(orderByField).arg(strSortOrder); //"order by xxx desc"
	}
	if (0 == DatabaseInstence->GetResultData(QString("SELECT * FROM %1 %2;").arg(m_stSqlInfo.tableName).arg(strOrderBy).toLocal8Bit(), ResultData))
	{
		outData = ResultData;
		return true;
	}
	strErrorMsg = QString::fromStdString(DatabaseInstence->GetLastError());
	return false;
}

bool DbScriptOperate::InsertData(const DbData& data, QString& strErrorMsg)
{
	return ExcuteDataOperateCommand(&DbScriptOperate::CreateInsertDataCommand, data, strErrorMsg);
}

bool DbScriptOperate::DeleteData(const DbData& data, QString& strErrorMsg)
{
	return ExcuteDataOperateCommand(&DbScriptOperate::CreateDeleteDataCommand, data, strErrorMsg);
}

bool DbScriptOperate::UpdateData(const DbData& data, QString& strErrorMsg)
{
	return ExcuteDataOperateCommand(&DbScriptOperate::CreateUpdateDataCommand, data, strErrorMsg);
}

QString DbScriptOperate::CreateInsertDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values)
{
	QString strFields;
	foreach(auto var, fields.fields)
	{
		strFields += QString("%1,").arg(var.value());
	}
	strFields = strFields.mid(0, strFields.length() - 1);

	QString strValues;
	foreach(auto cell, values.fields)
	{
		strValues += QString("\'%1\',").arg(cell.value());
	}
	strValues = strValues.mid(0, strValues.length() - 1);

	return QString("insert into %1 (%2) values (%3);").arg(m_stSqlInfo.tableName).arg(strFields).arg(strValues);
}

QString DbScriptOperate::CreateDeleteDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values)
{
	int fieldsCount = fields.fields.count();
	int valuesCount = values.fields.count();
	if (fieldsCount != valuesCount)
	{
		return "";
	}

	QString strPredicate;
	for (int i = 0; i < valuesCount; ++i)
	{
		strPredicate += QString("%1 = \'%2\' and ")
			.arg(fields.fields[i].value())
			.arg(values.fields[i].value());
	}
	strPredicate = strPredicate.mid(0, strPredicate.length() - 5);
	return QString("delete from %1 where %2;").arg(m_stSqlInfo.tableName).arg(strPredicate);
}

QString DbScriptOperate::CreateUpdateDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values)
{
	int fieldsCount = fields.fields.count();
	int valuesCount = values.fields.count();
	if (fieldsCount != valuesCount)
	{
		return "";
	}

	QString strPredicate;
	QString strValues;
	for (int i = 0; i < valuesCount; ++i)
	{
		DbDataCell cell = values.fields[i];
		DbDataCell field = fields.fields[i];
		if (cell.isWaitingUpdate())
		{
			strValues += QString("%1 = \'%2\',")
				.arg(field.value())
				.arg(cell.value());
		}
		else
		{
			strPredicate += QString("%1 = \'%2\' and ")
				.arg(field.value())
				.arg(cell.value());
		}
	}
	strValues = strValues.mid(0, strValues.length() - 1);
	strPredicate = strPredicate.mid(0, strPredicate.length() - 5);

	return QString("update %1 set %2 where %3;").arg(m_stSqlInfo.tableName)
		.arg(strValues).arg(strPredicate);
}

bool DbScriptOperate::ExcuteDataOperateCommand(const CreateCommandFunc createCommand, const DbData& data, QString& strErrorMsg)
{
	if (!CheckConnect(strErrorMsg))
	{
		return false;
	}

	int res = DatabaseInstence->BeginTransaction();
	if (0 == res)
	{
		foreach(auto row, data.rows)
		{
			if (row)
			{
				QString command((this->*createCommand)(data.fieldGroup, *row));
				res = DatabaseInstence->ExcuteCommand(command.toLocal8Bit());
				if (0 != res)
				{
					res = DatabaseInstence->RollBackTransaction(nullptr);
					strErrorMsg = QString::fromStdWString(L"命令：%1\r\n错误：%2").arg(command)
									.arg(QString::fromStdString(DatabaseInstence->GetQueryLastError()));
					return false;
				}
			}
		}
		return 0 == DatabaseInstence->CommitTransaction();
	}
	strErrorMsg = QString::fromStdString(DatabaseInstence->GetLastError());
	return false;
}
