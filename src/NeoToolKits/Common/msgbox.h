#ifndef MSGBOX_H
#define MSGBOX_H
 
#include <QObject>
#include<QEventLoop>
#include<QMessageBox>
#include<QApplication>
#include<QTimer>
#include<QMessageBox>

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
	static QMessageBox::StandardButton show(const QString& title, const QString& msg, const msgBoxType type);
	static QMessageBox::StandardButton LastUserSelectButton();
	void msgBox::testDemo();

private:
    const QString title;
    const QString msg;
	static QMessageBox::StandardButton m_LastUserSelectButton;
    int type;
    void readyShow(void);

public slots:
    void onShow();
};
 
#endif // MSGBOX_H