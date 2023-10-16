#include "DatabaseDataExportDbOperate.h"
#include "ExcelDataUploadApp.h"
using namespace NAMESPACENAME_DATABASE_DATA_EXPORT;

DatabaseDataExportDbOperate::DatabaseDataExportDbOperate(const SqlTableInfo& info)
	: ITableManage(info.baseInfo),
	m_stSqlInfo(info)
{}

DatabaseDataExportDbOperate::~DatabaseDataExportDbOperate()
{}

QString DatabaseDataExportDbOperate::CreateSelectDataCommand(const DbFieldGroup& fields, const DbFieldGroup& values)
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
	return QString("SELECT * FROM %1 WHERE %2;").arg(m_stSqlInfo.tableName).arg(strPredicate);
}

bool DatabaseDataExportDbOperate::ExcuteDataSelectCommand(const QString& cmd, DataTable& outputData, QString& strErrorMsg)
{
	if (!DatabaseInstence->IsInit())
	{
		strErrorMsg = QString::fromStdString(DatabaseInstence->GetLastError());
		return false;
	}

	int res = DatabaseInstence->BeginTransaction();
	if (0 == res)
	{
		if (!DatabaseInstence->GetResultData(cmd.toLocal8Bit(), outputData))
		{
			res |= DatabaseInstence->CommitTransaction();
		}
		else
		{
			res |= DatabaseInstence->RollBackTransaction(NULL);
			res |= DatabaseInstence->CommitTransaction();
		}
	}

	if (res != 0)
	{
		strErrorMsg = QString::fromStdString(DatabaseInstence->GetLastError());
		return false;
	}
	return true;
}


bool DatabaseDataExportDbOperate::QueryDataByIndexCondition(const ExportConfig& queryCfg, DataTable& outputData, QString& strErrorMsg)
{
	QString selectKeys;
	for each (auto var in queryCfg.exportFields)
	{
		selectKeys += var.DbKey;
		selectKeys += ",";
	}
	if (selectKeys.isEmpty())
	{
		selectKeys = "*";
	}
	else
	{
		selectKeys = selectKeys.mid(0, selectKeys.length() - 1);
	}

	QString selectSqlHead = QString("SELECT %1 FROM %2 ").arg(selectKeys, m_stSqlInfo.tableName);
	QString selectSql = QString("%1 WHERE ").arg(selectSqlHead);

	bool bFirstCondition = true;

	//通过箱号范围搜索数据
	QString BoxNumberStart(queryCfg.dataIndexCondition.BoxNumberStart);
	QString BoxNumberEnd(queryCfg.dataIndexCondition.BoxNumberEnd);
	if (!BoxNumberStart.isEmpty())
	{
		if (BoxNumberEnd.isEmpty())
		{
			selectSql += bFirstCondition ? "" : "AND ";
			selectSql += QString("package_no = \"%1\" ").arg(BoxNumberStart); //箱号字段名称
		}
		else
		{
			if (BoxNumberStart.length() > BoxNumberEnd.length())
			{
				strErrorMsg = QString::fromStdWString(L"末尾箱号长度小于起始箱号长度");
				return false;
			}
			auto BoxNumberEndArry = BoxNumberEnd.toLocal8Bit();
			auto BoxNumberStartArry = BoxNumberStart.toLocal8Bit();
			size_t indexDiff = 0;
			for (; indexDiff < BoxNumberStartArry.size(); indexDiff++)
			{
				if (BoxNumberStartArry[indexDiff] != BoxNumberEndArry[indexDiff])
				{
					break;
				}
			}
			QString strBoxNumberHead = BoxNumberStart.mid(0, indexDiff);
			QString strStartNum = BoxNumberStart.mid(indexDiff);
			QString strEndNum = BoxNumberEnd.mid(indexDiff);
			bool isNum = false;
			int iStartNum = strStartNum.toInt(&isNum);
			if (!isNum)
			{
				strErrorMsg = QString::fromStdWString(L"起始箱号流水号异常！");
				return false;
			}
			int iEndNum = strEndNum.toInt(&isNum);
			if (!isNum)
			{
				strErrorMsg = QString::fromStdWString(L"末尾箱号流水号异常！");
				return false;
			}
			int BoxNumberCount = iEndNum - iStartNum + 1; //箱号个数
			if (BoxNumberCount <= 0)
			{
				strErrorMsg = QString::fromStdWString(L"异常：起始箱号流水号 > 末尾箱号流水号！");
				return false;
			}

			selectSql += bFirstCondition ? "" : "AND ";
			for (size_t i = 0; i < BoxNumberCount; i++)
			{
				if (i == 0)
				{
					selectSql += "(";
				}
				QString strCurBoxNumber = QString("%1%2").arg(strBoxNumberHead).arg(iStartNum+i, strEndNum.length(), 10, QLatin1Char('0'));
				selectSql += QString("package_no = \"%1\" ").arg(strCurBoxNumber);
				if (i == BoxNumberCount-1)
				{
					selectSql += ") ";
				}
				else
				{
					selectSql += "OR ";
				}
			}
		}
		bFirstCondition = false;
	}

	//通过工单号搜索数据
	if (!queryCfg.dataIndexCondition.workOrderID.isEmpty())
	{
		selectSql += bFirstCondition ? "" : "AND ";
		selectSql += QString("work_order_id = \"%2\" ").arg(queryCfg.dataIndexCondition.workOrderID);
		bFirstCondition = false;
	}

	if (bFirstCondition)
	{
		strErrorMsg = QString::fromStdWString(L"查询条件不能为空");
		return false;
	}

	if (!BoxNumberStart.isEmpty() && selectKeys.contains("package_no"))
	{
		selectSql += QString("ORDER BY package_no ASC "); //根据箱号升序排序
	}

	return ExcuteDataSelectCommand(selectSql, outputData, strErrorMsg);
}


bool DatabaseDataExportDbOperate::test(QString& strErrMsg)
{
	if (!DatabaseInstence->IsInit())
	{
		strErrMsg = QString::fromStdWString(L"数据库连接失败：%1").arg(m_stSqlInfo.baseInfo.toString());
		return false;
	}

	if (!IsExistTable(m_stSqlInfo.tableName.toLocal8Bit()))
	{
		strErrMsg = QString::fromStdWString(L"数据表<%1>不存在").arg(m_stSqlInfo.tableName);
		return false;
	}

	return true;
}

