#include "mainwindow.h"
#include <QApplication>

//Just create  a application object and execute...
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
