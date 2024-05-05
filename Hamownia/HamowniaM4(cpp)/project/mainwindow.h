//Wiktor Preuss 2024
// For PUT Powertrain
//Alpha Bulid

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include "SpeedMeter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //ADDED FUNCTIONS
    void clearTextBoxes();
    void printLogBox();
    void resetData();
    void showDataInLogBox();
    void recordData();
    void saveToCSV();
    void rotateLabel();

    SpeedMeter *speedmeter1;
    SpeedMeter *speedmeter2;
    SpeedMeter *speedmeter3;
    SpeedMeter *speedmeter4;

private:
    Ui::MainWindow *ui;
    std::array<QTextBrowser*, 10> textBrowsers;
};
#endif // MAINWINDOW_H
