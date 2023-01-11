#pragma once
#include <QString>

#define NAMESPACENAME_DB_SCRIPT_EDITOR  Namespace_DbScriptEditor 

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	typedef struct _TestItemExcelInfo_
	{
		QString strExcelPath; //Excel路径
		QStringList listExcelColumns; //excel的标题列表

		int nColIndex_ItemCode; //Excel中测试项代号列序号
		QString strColName_ItemCode; //Excel中测试项代号列标题

		int nColIndex_ItemName; //Excel中测试项名称列序号
		QString strColName_ItemName; //Excel中测试项名称列标题
	
		bool isValid()
		{
			if (strExcelPath.isEmpty()
				|| nColIndex_ItemCode < 1
				|| nColIndex_ItemName < 1
				|| nColIndex_ItemCode == nColIndex_ItemName)
			{
				return false;
			}
			return true;
		}
		
	}TestItemExcelInfo, * pTestItemExcelInfo;

	typedef enum _TestItemOperate_
	{
		TestItem_NOP,
		TestItem_Insert,
		TestItem_Delete,
		TestItem_Update,
	}TestItemOperate;

	typedef struct _TestItem_
	{
	private:
		TestItemOperate eOperate;
		QString strCode;
		QString strName;

	public:
		explicit _TestItem_(const QString& code, const QString& name) {
			eOperate = TestItemOperate::TestItem_NOP;
			strCode = code;
			strName = name;
		}
		QString toString() const {
			return QString::fromStdWString(L"[%1][%2]").arg(strCode).arg(strName);
		}
		void setValue(const QString& code, const QString& name) {
			this->strCode = code;
			this->strName = name;
		}
		QString code() const {
			return this->strCode;
		}
		QString name() const {
			return this->strName;
		}
		bool isWaitingOperate() {
			return eOperate != TestItemOperate::TestItem_NOP;
		}
		void setUnOperate() {
			eOperate = TestItemOperate::TestItem_NOP;
		}
		bool isWaitingUpdate() const {
			return TestItemOperate::TestItem_Update == eOperate;
		}
		void setWaitingUpdate() {
			eOperate = TestItemOperate::TestItem_Update;
		}
		bool isWaitingDelete() const {
			return TestItemOperate::TestItem_Delete == eOperate;
		}
		void setWaitingDelete() {
			eOperate = TestItemOperate::TestItem_Delete;
		}
		bool isWaitingInsert() const {
			return TestItemOperate::TestItem_Insert == eOperate;
		}
		void setWaitingInsert() {
			eOperate = TestItemOperate::TestItem_Insert;
		}
		bool isValid(){
			if (this->strCode.isEmpty()
				|| this->strName.isEmpty()){
				return false;
			}
			return true;
		}
	}TestItem, * pTestItem;
};
