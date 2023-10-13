#ifndef MSGBOX_H
#define MSGBOX_H
 
#include <QObject>
#include<QEventLoop>
#include<QMessageBox>
#include<QApplication>
#include<QTimer>

class msgBox:public QObject
{
    Q_OBJECT

public:
	enum msgBoxType
	{
		information,
		critical,
		question,
		warning
	};

public:
    explicit msgBox(QObject *parent = nullptr);
	msgBox(const QString& title, const QString& msg, const msgBoxType type);
	static void show(const QString& title, const QString& msg, const msgBoxType type);
	void msgBox::testDemo();

private:
    const QString title;
    const QString msg;
    int type;
    void readyShow(void);

public slots:
    void onShow();
};
 
#endif // MSGBOX_H