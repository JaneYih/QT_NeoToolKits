#pragma once
#include <QObject>
#include <QString>
#include <QMetaType>
#include <QDebug>

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
};

class StringGroupItem
{
public:
	int index;
	QString text;
	static bool compareIndexLessThan(const StringGroupItem& item1, const StringGroupItem& item2)
	{
		return item1.index < item2.index;
	}
};

class TestItem : public QObject
{
	Q_OBJECT
public:
	enum TestItemOperate
	{
		TestItem_NOP,
		TestItem_Insert,
		TestItem_Delete,
		TestItem_Update,
		TestItem_Move,
	};
	Q_ENUM(TestItemOperate)

	TestItem() 
		: eOperate(TestItemOperate::TestItem_NOP),
		strCode(""),
		strName("")
	{}
	explicit TestItem(const QString& code, const QString& name)
		: eOperate(TestItemOperate::TestItem_NOP),
		strCode(code),
		strName(name)
	{}
	TestItem(const TestItem& src) {
		copy(src);
	}
	TestItem operator=(const TestItem& src) {
		if (this == &src) {
			return *this;
		}
		copy(src);
		return *this;
	}
	bool operator==(const TestItem& src) {
		return this->eOperate == src.Operate()
				&& this->strCode == src.code()
				&& this->strName == src.name();
	}
	bool operator!=(const TestItem& src) {
		return this->eOperate != src.Operate()
			|| this->strCode != src.code()
			|| this->strName != src.name();
	}
	void copy(const TestItem& src) {
		this->eOperate = src.Operate();
		this->strCode = src.code();
		this->strName = src.name();
	}
	QString toString() const {
		return QString::fromStdWString(L"[%1]<------->[%2]").arg(strName).arg(strCode);
	}
	void fromString(const QString& str) {
		int firstIndex = str.indexOf('[') + 1;
		int lastIndex = str.indexOf(']');
		if (firstIndex <= 0 || lastIndex <= 0 || firstIndex > lastIndex)
		{
			return;
		}
		this->strName = str.mid(firstIndex, lastIndex - firstIndex);

		QString strTemp = str.mid(lastIndex+1);
		firstIndex = strTemp.indexOf('[') + 1;
		lastIndex = strTemp.indexOf(']');
		if (firstIndex <= 0 || lastIndex <= 0 || firstIndex > lastIndex)
		{
			return;
		}
		this->strCode = strTemp.mid(firstIndex, lastIndex - firstIndex);
	}
	void setValue(const QString& code, const QString& name) {
		this->strCode = code;
		this->strName = name;
	}
	QString code() const {
		return this->strCode;
	}
	void setCode(const QString& code) {
		this->strCode = code;
	}
	QString name() const {
		return this->strName;
	}
	void setName(const QString& name) {
		this->strName = name;
	}
	TestItemOperate Operate() const {
		return this->eOperate;
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
	bool isWaitingMove() const {
		return TestItemOperate::TestItem_Move == eOperate;
	}
	void setWaitingMove() {
		eOperate = TestItemOperate::TestItem_Move;
	}
	bool isValid() {
		if (this->strCode.isEmpty()
			|| this->strName.isEmpty()) {
			return false;
		}
		return true;
	}

private:
	TestItemOperate eOperate;
	QString strCode;
	QString strName;
};

Q_DECLARE_METATYPE(TestItem);
extern QDebug operator<<(QDebug dbg, const TestItem& item);