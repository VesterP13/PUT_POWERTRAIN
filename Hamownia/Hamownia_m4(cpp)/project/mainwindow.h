//Wiktor Preuss 2024
// For PUT Powertrain
//Alpha Bulid

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTextBrowser>
#include <vector>
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
private:
    Ui::MainWindow *ui;
    void clearTextBoxes();
    void printLogBox();
    void resetData();
    void showDataInLogBox();
    void UpdateMeasurments();
    void recordData();
    void saveToCSV();
    void rotateLabel();
    void setupWidgets();
    void setupButtons();
    void setupDataHandling();
    void setupTheme();
    void showDataInConsole();
    void setTextColorInTextBrowsers(QColor color);
    bool recordingData = false;
    void setButtonState(std::string command);
    SpeedMeter *speedmeterLeftUp;
    SpeedMeter *speedmeterLeftDown;
    SpeedMeter *speedmeterRightUp;
    SpeedMeter *speedmeterRightDown;
    std::vector<int> rpmVector_m1 = {1000, 2000, 3000};
    std::vector<int> rpmVector_m2 = {1000, 2000, 3000};
    std::vector<int> rpmVector_m3 = {1000, 2000, 3000};
    std::vector<int> rpmVector_m4 = {1000, 2000, 3000};
    std::vector<int> currentVector_m1 = {10, 11, 12, 13};
    std::vector<int> currentVector_m2 = {10, 11, 12, 13};
    std::vector<int> currentVector_m3 = {10, 11, 12, 13};
    std::vector<int> currentVector_m4 = {10, 11, 12, 13};
    std::vector<int> ambientTemperature = {20, 21, 22, 23};
    std::vector<int> batteryTemperature = {40, 41, 42, 43};
    std::array<QTextBrowser*, 10> textBrowsers;
    std::array<std::vector<int>*, 10> dataArray = {&rpmVector_m1, &rpmVector_m2, &rpmVector_m3, &rpmVector_m4, &currentVector_m1, &currentVector_m2, &currentVector_m3, &currentVector_m4, &ambientTemperature, &batteryTemperature};


};
#endif // MAINWINDOW_H
