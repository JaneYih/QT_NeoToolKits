#include "ExcelDataUploadDbOperate.h"
#include "ExcelDataUploadApp.h"
using namespace NAMESPACENAME_EXCEL_DATA_UPLOAD;

ExcelDataUploadDbOperate::ExcelDataUploadDbOperate(const SqlTableInfo& info)
	: ITableManage(info.baseInfo),
	m_stSqlInfo(info)
{
	
}

ExcelDataUploadDbOperate::~ExcelDataUploadDbOperate()
{
}

bool ExcelDataUploadDbOperate::InsertExcelData(const QList<QVector<UploadData>>& dataList, 
	const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo)
{
	return OperateExcelData(cmdType::InsertCommand, dataList, stUploadConfig, pstUploadingInfo);
}

bool ExcelDataUploadDbOperate::UpdateExcelData(const QList<QVector<UploadData>>& dataList,
	const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo)
{
	return OperateExcelData(cmdType::UpdateCommand, dataList, stUploadConfig, pstUploadingInfo);
}

bool ExcelDataUploadDbOperate::OperateExcelData(cmdType type, const QList<QVector<UploadData>>& dataList,
	const ExcelDataUploadConfig& stUploadConfig, pUploadingInfo pstUploadingInfo)
{
	if (!DatabaseInstence->IsInit())
	{
		return false;
	}

	int res = DatabaseInstence->BeginTransaction();
	int count = 0;
	int countMax = dataList.count();
	foreach(auto row, dataList)
	{
		QString command;
		QString displayText;
		switch (type)
		{
		case InsertCommand:
			command = GenerateInsertCommand(m_stSqlInfo.tableName, row, displayText);
			break;
		case UpdateCommand:
			command = GenerateUpdateCommand(m_stSqlInfo.tableName, row, stUploadConfig.strProductionOrderID, displayText);
			break;
		default:
			return false;
		}

		std::string strErrMsg;
		res |= DatabaseInstence->ExcuteCommand(command.toLocal8Bit());
		if (res == 0)
		{
			DataTable ResultData;
			res |= DatabaseInstence->GetResultData("SELECT ROW_COUNT();", ResultData);
			if (res == 0
				&& ResultData.RowList.size() >= 1
				&& ResultData.RowList[0].FieldListValue.size() >= 1)
			{
				std::string rowconut = ResultData.RowList[0].FieldListValue[0];
				if (rowconut == "0")
				{
					res = 1;
					strErrMsg = "Affected row 0";
				}
			}
			else
			{
				strErrMsg = "The execution result query failed!";
			}
		}
		else
		{
			strErrMsg = DatabaseInstence->GetLastError();
		}

		QString strTemp = (res == 0)
			? QString::fromStdWString(L"执行成功【%1】").arg(displayText)
			: QString::fromStdWString(L"执行失败【%1】：<error:%2>,<%3>").arg(displayText).arg(QString::fromStdString(strErrMsg)).arg(command);
		emit(static_cast<ExcelDataUploadApp*>(stUploadConfig.pApp))->toDisplayItem(strTemp, (res == 0) ? ++count : count, countMax);

		if ((stUploadConfig.bErrorStop && res)
			|| pstUploadingInfo->bStop)
		{
			DatabaseInstence->RollBackTransaction(NULL);
			DatabaseInstence->CommitTransaction();
			emit(static_cast<ExcelDataUploadApp*>(stUploadConfig.pApp))->toDisplayItem(QString::fromStdWString(L"回滚此次的所有操作。总数：%1，成功：0，失败：0").arg(countMax), 0, countMax);
			return false;
		}
	}
	int commitRes = DatabaseInstence->CommitTransaction();
	if (commitRes == 0)
	{
		emit(static_cast<ExcelDataUploadApp*>(stUploadConfig.pApp))->toDisplayItem(QString::fromStdWString(L"执行完毕，总数：%1，成功：%2，失败：%3").arg(countMax).arg(count).arg(countMax-count), count, countMax);
	}
	else
	{
		emit(static_cast<ExcelDataUploadApp*>(stUploadConfig.pApp))->toDisplayItem(QString::fromStdWString(L"提交失败。总数：%1，成功：0，失败：0").arg(countMax), 0, countMax);
	}
	res |= commitRes;
	return !res;
}

QString ExcelDataUploadDbOperate::GenerateInsertCommand(const QString& tableName, 
	const QVector<UploadData>& data,
	QString& displayText)
{
	QString strKeys;
	QString strValues;
	foreach(auto var, data)
	{
		strKeys += QString("%1,").arg(var.key);
		strValues += QString("\"%1\",").arg(var.value);
		displayText += QString("[%1]<<\"%2\";").arg(var.key).arg(var.value);
	}
	strKeys = strKeys.left(strKeys.length() - 1);
	strValues = strValues.left(strValues.length() - 1);

	QString command = QString("INSERT INTO %1(%2) VALUES(%3);")
		.arg(tableName)
		.arg(strKeys)
		.arg(strValues);

	return command;
}

QString ExcelDataUploadDbOperate::GenerateUpdateCommand(const QString& tableName, 
	const QVector<UploadData>& data, 
	const QString& strProductionOrderID,
	QString& displayText)
{
	QString strSets;
	QString strCondition;
	foreach(auto var, data)
	{
		strSets += QString("%1=\"%2\",").arg(var.key, var.value);
		strCondition += QString("(%1 IS NULL OR %1=\"\") AND ").arg(var.key);
		displayText += QString("[%1]<<\"%2\";").arg(var.key).arg(var.value);
	}
	strSets = strSets.left(strSets.length() - 1);
	strCondition += QString("work_order_id=\"%1\"").arg(strProductionOrderID);

	QString command = QString("UPDATE %1 SET %2 WHERE %3 LIMIT 1;")
		.arg(tableName)
		.arg(strSets)
		.arg(strCondition);

	return command;
}

bool ExcelDataUploadDbOperate::test(QString& strErrMsg)
{
	if (!DatabaseInstence->IsInit())
	{
		strErrMsg = QString::fromStdWString(L"数据库连接失败：%1").arg(m_stSqlInfo.baseInfo.toString());
		return false;
	}

	if (!ITableManage::IsExistTable(m_stSqlInfo.tableName.toLocal8Bit()))
	{
		strErrMsg = QString::fromStdWString(L"数据表<%1>不存在").arg(m_stSqlInfo.tableName);
		return false;
	}

	return true;
}

