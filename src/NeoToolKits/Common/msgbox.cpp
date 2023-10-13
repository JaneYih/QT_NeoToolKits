#include "msgbox.h"
 
msgBox::msgBox(QObject *parent):QObject (parent)
{
 
}
 
void msgBox::testDemo()
{
	msgBox::show("info", "operaiton is ok", msgBoxType::information);
	msgBox::show("info", "operaiton is ok", msgBoxType::critical);
	msgBox::show("info", "operaiton is ok", msgBoxType::question);
}

void msgBox::readyShow()
{
    this->moveToThread(qApp->thread());
    QTimer::singleShot(0,this,SLOT(onShow()));
}
 
msgBox::msgBox(const QString &title, const QString &msg, const msgBoxType type)
    : title(title),
    msg(msg),
    type(type)
{
 
}
 
void msgBox::show(const QString &title, const QString &msg,const msgBoxType type)
{
    QEventLoop eventLoop;
    auto messageBox = new msgBox(title,msg,type);
    connect(messageBox,SIGNAL(destroyed()),&eventLoop,SLOT(quit()));
    messageBox->readyShow();
    eventLoop.exec();
}
 
void msgBox::onShow()
{
    switch (type)
    {
    case msgBoxType::information:
        QMessageBox::information(NULL, title, msg);
        break;
    case msgBoxType::critical:
        QMessageBox::critical(NULL, title, msg);
        break;
    case msgBoxType::question:
        QMessageBox::question(NULL, title, msg);
        break;
	case msgBoxType::warning:
		QMessageBox::warning(NULL, title, msg);
		break;
    }
    this->deleteLater();
}