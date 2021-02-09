#include "mainwindow.h"
#include "translatecore.h"

#include <QApplication>
#include <QDebug>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto t=QStyleFactory::create("Fusion");
    a.setStyle(t);
    a.setWindowIcon(QIcon(":/icons/langSword.svg"));
    MainWindow w;
    return a.exec();
}
