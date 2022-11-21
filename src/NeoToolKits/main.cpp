#include "MainWindowDialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindowDialog w;
    w.show();
    return a.exec();
}
