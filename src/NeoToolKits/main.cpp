#include "MainWindowDialog.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>
#include "DumpFile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeclareDumpFile();

    MainWindowDialog w;
    w.show();
    if (argc == 2)
    {
        QString strFileName = QString::fromLocal8Bit(argv[1]);
        if (!w.OpenDbScript(strFileName))
        {
			QMessageBox::warning(&w, "warning", QString::fromStdWString(L"文件打开失败：%1").arg(strFileName));
        }
    }
    return a.exec();
}
