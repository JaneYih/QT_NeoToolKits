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
	bool bExportNothing = true;
	for each (auto var in queryCfg.exportFields)
	{
		selectKeys += var.DbKey;
		selectKeys += ",";

		if (bExportNothing && var.bExport)
		{
			bExportNothing = false;
		}
	}
	if (selectKeys.isEmpty() || bExportNothing)
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
		if (!packageSqlBySerialNumberCondition(selectSql, bFirstCondition,
			QString::fromStdWString(L"箱号"), "package_no",
			BoxNumberStart, BoxNumberEnd, strErrorMsg))
		{
			return false;
		}
	}

	//通过托盘号范围搜索数据
	QString TrayNoStart(queryCfg.dataIndexCondition.TrayNoStart);
	QString TrayNoEnd(queryCfg.dataIndexCondition.TrayNoEnd);
	if (!TrayNoStart.isEmpty())
	{
		if (!packageSqlBySerialNumberCondition(selectSql, bFirstCondition,
			QString::fromStdWString(L"托盘号"), "tray_no",
			TrayNoStart, TrayNoEnd, strErrorMsg))
		{
			return false;
		}
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
		//strErrorMsg = QString::fromStdWString(L"查询条件不能为空");
		//return false;
		selectSql = QString("%1 LIMIT 5 ").arg(selectSqlHead);
	}

	if (!BoxNumberStart.isEmpty() && selectKeys.contains("package_no"))
	{
		selectSql += QString("ORDER BY package_no ASC "); //根据箱号升序排序
	}
	if (!TrayNoStart.isEmpty() && selectKeys.contains("tray_no"))
	{
		//根据托盘号升序排序
		if (selectSql.contains("ORDER BY"))
		{
			selectSql += QString(", tray_no ASC ");
		}
		else
		{
			selectSql += QString("ORDER BY tray_no ASC "); 
		}
	}

	return ExcuteDataSelectCommand(selectSql, outputData, strErrorMsg);
}


bool DatabaseDataExportDbOperate::packageSqlBySerialNumberCondition(QString& selectSql, bool& bFirstCondition,
	const QString& name, const QString& dbKey,
	const QString& startNum, const QString& endNum,
	QString& strErrorMsg)
{
	if (endNum.isEmpty() || startNum == endNum)
	{
		selectSql += bFirstCondition ? "" : "AND ";
		selectSql += QString("%1 = \"%2\" ").arg(dbKey).arg(startNum); //字段名称
	}
	else
	{
		if (startNum.length() > endNum.length())
		{
			strErrorMsg = QString::fromStdWString(L"末尾%1长度小于起始%1长度").arg(name);
			return false;
		}
		auto EndArry = endNum.toLocal8Bit();
		auto StartArry = startNum.toLocal8Bit();
		size_t indexDiff = 0;
		for (; indexDiff < StartArry.size(); indexDiff++)
		{
			if (StartArry[indexDiff] != EndArry[indexDiff])
			{
				break;
			}
		}
		QString strNumberHead = startNum.mid(0, indexDiff);
		QString strStartNum = startNum.mid(indexDiff);
		QString strEndNum = endNum.mid(indexDiff);
		bool isNum = false;
		int iStartNum = strStartNum.toInt(&isNum);
		if (!isNum)
		{
			strErrorMsg = QString::fromStdWString(L"起始%1流水号异常！").arg(name);
			return false;
		}
		int iEndNum = strEndNum.toInt(&isNum);
		if (!isNum)
		{
			strErrorMsg = QString::fromStdWString(L"末尾%1流水号异常！").arg(name);
			return false;
		}
		int NumberCount = iEndNum - iStartNum + 1; //个数
		if (NumberCount <= 0)
		{
			strErrorMsg = QString::fromStdWString(L"异常：起始%1流水号 > 末尾%1流水号！").arg(name);
			return false;
		}

		selectSql += bFirstCondition ? "" : "AND ";
		for (size_t i = 0; i < NumberCount; i++)
		{
			if (i == 0)
			{
				selectSql += "(";
			}
			QString strCurNumber = QString("%1%2").arg(strNumberHead).arg(iStartNum + i, strEndNum.length(), 10, QLatin1Char('0'));
			selectSql += QString("%1 = \"%2\" ").arg(dbKey).arg(strCurNumber);
			if (i == NumberCount - 1)
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
	return true;
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

