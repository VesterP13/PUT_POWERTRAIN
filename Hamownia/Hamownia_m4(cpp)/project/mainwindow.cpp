//Wiktor Preuss 2024
// For PUT Powertrain
//Alpha Bulid
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <QPixmap>
#include<QPainter>
#include <QPaintEvent>
#include <QPalette>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupDataHandling();
    setupTheme();
    setupWidgets();
    setupButtons();
    //speedmeterLeftDown->simulation(100,100);
    //speedmeterLeftUp->simulation(100,100);
    //speedmeterRightDown->simulation(100,100);
    //speedmeterRightUp->simulation(100,100);
}

void MainWindow::setupTheme(){
    QLinearGradient gradient(0, 0, 0, height());
    QColor color = ("#050317");
    gradient.setColorAt(0, color);
    color = ("#090C38");
    gradient.setColorAt(0.5, color);
    color = ("#050317");
    gradient.setColorAt(1, color);
    QPalette palette;
    palette.setBrush(QPalette::Window, gradient);
    palette.setBrush(QPalette::Base, gradient);
    palette.setBrush(QPalette::AlternateBase, gradient);
    setPalette(palette);
    ui->textBrowser_console->setPalette(palette);
    ui->textBrowser_console->setStyleSheet("QTextBrowser { background: transparent;}");
    for (QTextBrowser* textBrowser : this->textBrowsers)
    {
        textBrowser->setStyleSheet("QTextBrowser { background: transparent;}");
    }
}

void MainWindow::setupButtons(){
    connect(ui->exportDataButton, &QPushButton::clicked, this, &MainWindow::saveToCSV);
    connect(ui->resetDataButton, &QPushButton::clicked, this, &MainWindow::UpdateMeasurments);
    connect(ui->clearLogBoxButton, &QPushButton::clicked, this, &MainWindow::clearTextBoxes);
    connect(ui->recordDataButton, &QPushButton::clicked, this, &MainWindow::recordData);
}

void MainWindow::setupDataHandling(){
    textBrowsers={ui->textBrowser_rpm_m1,
                    ui->textBrowser_rpm_m2,
                    ui->textBrowser_rpm_m3,
                    ui->textBrowser_rpm_m4,
                    ui->textBrowser_current_m1,
                    ui->textBrowser_current_m2,
                    ui->textBrowser_current_m3,
                    ui->textBrowser_current_m4,
                    ui->textBrowser_ambient_temperature,
                    ui->textBrowser_battery_temperature};
}

void MainWindow::setupWidgets(){
    speedmeterLeftUp = new SpeedMeter(this);
    speedmeterLeftDown = new SpeedMeter(this);
    speedmeterRightUp = new SpeedMeter(this);
    speedmeterRightDown = new SpeedMeter(this);


    ui->gridLayout_main->addWidget(speedmeterLeftUp, 0, 0);
    ui->gridLayout_main->addWidget(speedmeterRightUp, 0, 2);
    ui->gridLayout_main->addWidget(ui->line, 0, 1);
    ui->gridLayout_main->addWidget(ui->line_2, 1, 1);
    ui->gridLayout_main->addWidget(speedmeterLeftDown, 1, 0);
    ui->gridLayout_main->addWidget(speedmeterRightDown, 1, 2);
}

void MainWindow::clearTextBoxes()
{
    for (QTextBrowser* textBrowser : this->textBrowsers)
        {
            textBrowser->clear();
        }
}

void MainWindow::resetData()
{
    for (std::vector<int>* vector : dataArray) {
        vector->clear();
    }
}

void MainWindow::UpdateMeasurments(){
    showDataInLogBox();
    showDataInConsole();
}

void MainWindow::showDataInLogBox()
{
    ui->textBrowser_rpm_m1->append(QString::number(rpmVector_m1.back()));
    ui->textBrowser_rpm_m2->append(QString::number(rpmVector_m2.back()));
    ui->textBrowser_rpm_m3->append(QString::number(rpmVector_m3.back()));
    ui->textBrowser_rpm_m4->append(QString::number(rpmVector_m4.back()));
    ui->textBrowser_current_m1->append(QString::number(currentVector_m1.back()));
    ui->textBrowser_current_m2->append(QString::number(currentVector_m2.back()));
    ui->textBrowser_current_m3->append(QString::number(currentVector_m3.back()));
    ui->textBrowser_current_m4->append(QString::number(currentVector_m4.back()));
    ui->textBrowser_ambient_temperature->append(QString::number(ambientTemperature.back()) + "°C");
    ui->textBrowser_battery_temperature->append(QString::number(batteryTemperature.back()) + "°C");
}

void MainWindow::showDataInConsole(){
    ui->textBrowser_console->append(QString::number(rpmVector_m1.back()));
}

void MainWindow::recordData()
{
    if (recordingData == false)
    {
        setTextColorInTextBrowsers("green");
        setButtonState("Stop");
        recordingData = true;
    }
    else
    {
        setTextColorInTextBrowsers("white");
        setButtonState("Nagrywaj");
        recordingData = false;
    }
}

void MainWindow::setButtonState(std::string command) {
    if (command != "Stop" && command != "Nagrywaj") {
        return;
    }
    else if (command == "Stop") {
        ui->recordDataButton->setPalette(QPalette(Qt::darkRed));
        ui->recordDataButton->setText("Stop");
    }
    else {
        ui->recordDataButton->setPalette(QPalette(Qt::darkGreen));
        ui->recordDataButton->setText("Nagrywaj");
    }
}

void MainWindow::setTextColorInTextBrowsers(QColor color){
    for (QTextBrowser* textBrowser : this->textBrowsers)
    {
        textBrowser->setTextColor(color);
    }
}

std::string filename = "xd";

void MainWindow::saveToCSV()
{
    std::ofstream file(filename);

    file << "RPM_M1;RPM_M2;RPM_M3;RPM_M4;I_M1;I_M2;I_M3;I_M4;AMBNT_TEMP;BATT_TEMP;";
    file << std::endl;
    file << "[obr/min];[obr/min];[obr/min];[obr/min];[A];[A];[A];[A];[C];[C];";
    file << std::endl;

    for (int i = 0; i < (dataArray[0]->size()); i++)
    {
        for (std::vector<int>* vector : dataArray) {
            file << (*vector)[i];
            file << ";";
        }
        file << std::endl;
    }

    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
