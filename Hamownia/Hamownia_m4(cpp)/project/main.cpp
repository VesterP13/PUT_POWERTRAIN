//Wiktor Preuss 2024
// For PUT Powertrain
//Alpha Bulid

#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

void windowStandardSetup(MainWindow &window);
void windowFullScreenSetup(MainWindow &window);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    windowStandardSetup(window);
    //windowFullScreenSetup(window);

    return a.exec();
}

void windowStandardSetup(MainWindow &window){
    //window.setGeometry(0, 0, 1024, 600);
    window.setMinimumWidth(1024);
    window.setMinimumHeight(600);
    window.show();
}

void windowFullScreenSetup(MainWindow &window){
    QScreen *primaryScreen  = QGuiApplication::primaryScreen();
    QSize screenSize = primaryScreen->size();
    window.setGeometry(0, 0, screenSize.width(), screenSize.height());
    window.show();
}



