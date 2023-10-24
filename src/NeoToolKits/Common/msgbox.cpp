#include "msgbox.h"
 
QMessageBox::StandardButton msgBox::m_LastUserSelectButton = QMessageBox::NoButton;

msgBox::msgBox(QObject *parent):QObject (parent)
{
 
}
 
void msgBox::testDemo()
{
	msgBox::show("info", "operaiton is ok", msgBoxType::information);
	msgBox::show("info", "operaiton is ok", msgBoxType::critical);
    QMessageBox::StandardButton res = msgBox::show("info", "operaiton is ok", msgBoxType::question);
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
 
QMessageBox::StandardButton msgBox::show(const QString &title, const QString &msg,const msgBoxType type)
{
    QEventLoop eventLoop;
    auto messageBox = new msgBox(title,msg,type);
    connect(messageBox,SIGNAL(destroyed()),&eventLoop,SLOT(quit()));
    messageBox->readyShow();
    eventLoop.exec();
    return LastUserSelectButton();
}
 
void msgBox::onShow()
{
    switch (type)
    {
    case msgBoxType::information:
        m_LastUserSelectButton = QMessageBox::information(NULL, title, msg);
        break;
    case msgBoxType::critical:
        m_LastUserSelectButton = QMessageBox::critical(NULL, title, msg);
        break;
    case msgBoxType::question:
        m_LastUserSelectButton = QMessageBox::question(NULL, title, msg);
        break;
	case msgBoxType::warning:
        m_LastUserSelectButton = QMessageBox::warning(NULL, title, msg);
		break;
    }
    this->deleteLater();
}

QMessageBox::StandardButton msgBox::LastUserSelectButton()
{
    return m_LastUserSelectButton;
}