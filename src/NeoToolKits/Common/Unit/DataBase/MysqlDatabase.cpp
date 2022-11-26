#include "MysqlDatabase.h"

CMysqlDatabase::CMysqlDatabase(
	const string& host,
	const string& user,
	const string& passwd,
	const string& db,
	const unsigned int port) :InitErr(0)
{
	m_strServiceIp = host; 
	m_strLoginName = user;       
	m_strLoginPassword = passwd; 
	m_strDataBaseName = db;  
	m_iLoginPort = port;

	InitErr = Init();
	InitializeCriticalSection(&m_Critical); 
	InitializeCriticalSection(&m_Transaction_Critical);
}

CMysqlDatabase::~CMysqlDatabase()
{
	UnInit();
	DeleteCriticalSection(&m_Critical); 
	DeleteCriticalSection(&m_Transaction_Critical);
}

bool CMysqlDatabase::IsInit()
{
	return !InitErr;
}

//0:成功   其他:错误码
int CMysqlDatabase::Init()
{
	if (mysql_library_init(0, NULL, NULL)) 
	{
		return 1;
	}

	mysql_init(&m_mySql);
	if (&m_mySql == NULL)
	{
		return 2;
	}

	if (mysql_options(&m_mySql, MYSQL_SET_CHARSET_NAME, "gbk"))
	{
		return 3;
	}

	if (NULL == mysql_real_connect(&m_mySql, 
					m_strServiceIp.c_str(), m_strLoginName.c_str(), m_strLoginPassword.c_str(), 
					m_strDataBaseName.c_str(), m_iLoginPort, NULL, 0)) // 连接到MySQL服务器。
	{
		return 4;
	}

	return 0;
}
int CMysqlDatabase::UnInit() 
{
	mysql_close(&m_mySql);  
	mysql_library_end(); 
	return 0;
}

int CMysqlDatabase::ExcuteCommand(const char*  command)
{
	int errNum = 0;
	int Len = 0;
	while (command[Len++] != '\0'){}
	EnterCriticalSection(&m_Critical);
	errNum = mysql_real_query(&m_mySql, command, Len);
	LeaveCriticalSection(&m_Critical); 
	return errNum;
}

//获取查询的数据,返回错误码
int CMysqlDatabase::GetResultData(const char*  command, DataTable& ResultData)
{
	int errNum = ExcuteCommand(command);
	if (errNum != 0)
	{
		return errNum;
	}
	
	int FieldCount = mysql_field_count(&m_mySql);

	if (FieldCount)
	{
		MYSQL_RES *result;
		ResultData.FieldName.FieldListValue.clear();
		ResultData.RowList.clear();

		result = mysql_store_result(&m_mySql);

		//遍历数据
		struct st_mysql_rows *head = result->data->data;
		struct st_mysql_rows *next = head;

		//字段行
		for (int i = 0; i < FieldCount; i++)   //字段值
		{
			string field_Name = "";
			if (result->fields[i].name != NULL)
			{
				field_Name = result->fields[i].name;
			}
			ResultData.FieldName.FieldListValue.push_back(result->fields[i].name);
		}

		//具体数据表装载
		for (int j = 0; j < result->row_count; j++)
		{
			FieldList RowValue;
			RowValue.FieldListValue.clear();
			for (int i = 0; i < FieldCount; i++)   //字段值
			{
				string field_Value = "";
				if (next->data[i] != NULL)
				{
					field_Value = next->data[i];
				}
				
				RowValue.FieldListValue.push_back(field_Value);
			}

			ResultData.RowList.push_back(RowValue);
			next = next->next;
		}
		mysql_free_result(result);
	}
	else
	{
		return -1;
	}

	return 0;
}

int CMysqlDatabase::BeginTransaction()
{
	int err = ExcuteCommand("START TRANSACTION;");
	if (0 == err)
	{
		EnterCriticalSection(&m_Transaction_Critical);
	}
	return err;
}

int CMysqlDatabase::RollBackTransaction(const char* PointName)
{
	string	strRollbackSql = "ROLLBACK TO %s;";
	char command[1024] = { 0 };
	sprintf_s(command, strRollbackSql.c_str(), PointName);

	int err = 0;
	if (PointName == NULL)
	{
		err = ExcuteCommand("ROLLBACK;");
	}
	else
	{
		err = ExcuteCommand(command);
	}

	return err;
}

int CMysqlDatabase::CommitTransaction()
{
	int err = ExcuteCommand("COMMIT;");
	LeaveCriticalSection(&m_Transaction_Critical);
	return err;
}

int CMysqlDatabase::SetRollBackPoint(const char* PointName)
{
	string	strSetRollbackPointSql = "SAVEPOINT %s;";
	char command[1024] = { 0 };
	sprintf_s(command, strSetRollbackPointSql.c_str(), PointName);
	int err = ExcuteCommand(command);
	return err;
}

/*索引*/
//建立单列索引，  表名、字段名、索引名   
int CMysqlDatabase::CreateIndex(const char* TableName, const char* FieldName, const char* IndexName)
{
	string strCreateIndexSql = "CREATE INDEX %s ON %s(%s ASC);";
	char command[1024] = { 0 };
	sprintf_s(command, strCreateIndexSql.c_str(), IndexName, TableName, FieldName);
	int err = ExcuteCommand(command);
	return err;
}

//删除单列索引，  表名、字段名、索引子名    (注：索引名=表名.索引子名)
int CMysqlDatabase::DeleteIndex(const char* TableName, const char* FieldName, const char* IndexName)
{
	string strFieldName = FieldName;
	string strDeleteIndexSql = "DROP INDEX %s ON %s;";
	char command[1024] = { 0 };
	sprintf_s(command, strDeleteIndexSql.c_str(), IndexName, TableName);

	int err = ExcuteCommand(command);

	return err;
}

//查询表索引
int CMysqlDatabase::ShowIndex(const char* TableName)
{
	string strShowIndexSql = "SHOW INDEX FROM %s;";
	char command[1024] = { 0 };
	sprintf_s(command, strShowIndexSql.c_str(), TableName);

	int err = ExcuteCommand(command);

	return err;
}

//数据表是否已存在
bool CMysqlDatabase::IsExistTable(const char* TableName)
{
	char command[1024] = { 0 };
	sprintf_s(command, "SHOW TABLES LIKE \'%s\'", TableName);
	
	DataTable OutData;
	OutData.RowList.clear();
	if (!GetResultData(command, OutData))
	{
		{if (OutData.RowList.size() >= 1)

			return true;    //数据表已存在
		}
	}

	return false;
}