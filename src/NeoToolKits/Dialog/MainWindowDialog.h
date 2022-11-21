#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindowDialog.h"

class MainWindowDialog : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowDialog(QWidget *parent = nullptr);
    ~MainWindowDialog();

private:
    Ui::MainWindowDialogClass ui;
};
