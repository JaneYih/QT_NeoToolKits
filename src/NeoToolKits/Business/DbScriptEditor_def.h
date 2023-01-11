#pragma once
#include <QString>

#define NAMESPACENAME_DB_SCRIPT_EDITOR  Namespace_DbScriptEditor 

namespace NAMESPACENAME_DB_SCRIPT_EDITOR
{
	typedef struct _TestItemExcelInfo_
	{
		QString strExcelPath; //Excel·��
		QStringList listExcelColumns; //excel�ı����б�

		int nColIndex_ItemCode; //Excel�в�������������
		QString strColName_ItemCode; //Excel�в���������б���

		int nColIndex_ItemName; //Excel�в��������������
		QString strColName_ItemName; //Excel�в����������б���
	
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
