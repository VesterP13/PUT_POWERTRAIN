//Wiktor Preuss 2024
// For PUT Powertrain
//Alpha Bulid

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    window.setGeometry(100, 100, 1024, 600);
    window.setMinimumHeight(600);
    window.setMinimumWidth(1024);
    //window.setMaximumHeight(600);
    //window.setMaximumWidth(1024);




    return a.exec();
}
