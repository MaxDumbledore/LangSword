#include "mainwindow.h"
#include "translatecore.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/langSword.svg"));
    MainWindow w;
    return a.exec();
}
