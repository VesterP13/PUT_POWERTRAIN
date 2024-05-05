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


std::array<std::vector<int>*, 10> dataArray = {&rpmVector_m1, &rpmVector_m2, &rpmVector_m3, &rpmVector_m4, &currentVector_m1, &currentVector_m2, &currentVector_m3, &currentVector_m4, &ambientTemperature, &batteryTemperature};

bool recordingData = false;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //textBrowsers = {ui->textBrowser_rpm_m1, ui->textBrowser_rpm_m2, ui->textBrowser_rpm_m3, ui->textBrowser_rpm_m4, ui->textBrowser_current_m1, ui->textBrowser_current_m2, ui->textBrowser_current_m3, ui->textBrowser_current_m4, ui->textBrowser_ambient_temperature, ui->textBrowser_battery_temperature};

    speedmeter1 = new SpeedMeter(this);
    speedmeter2 = new SpeedMeter(this);
    speedmeter3 = new SpeedMeter(this);
    speedmeter4 = new SpeedMeter(this);

    speedmeter1->simulation(-20);
    speedmeter2->simulation(20);
    speedmeter3->simulation(20);
    speedmeter4->simulation(-20);



    ui->gridLayout_main->addWidget(speedmeter1, 0, 0);
    ui->gridLayout_main->addWidget(speedmeter2, 0, 2);
    ui->gridLayout_main->addWidget(ui->line, 0, 1);
    ui->gridLayout_main->addWidget(ui->line_2, 1, 1);
    ui->gridLayout_main->addWidget(speedmeter3, 1, 0);
    ui->gridLayout_main->addWidget(speedmeter4, 1, 2);

    connect(ui->exportDataButton, &QPushButton::clicked, this, &MainWindow::saveToCSV);
    connect(ui->resetDataButton, &QPushButton::clicked, this, &MainWindow::showDataInLogBox);
    connect(ui->clearLogBoxButton, &QPushButton::clicked, this, &MainWindow::clearTextBoxes);
    connect(ui->recordDataButton, &QPushButton::clicked, this, &MainWindow::recordData);

}

MainWindow::~MainWindow()
{
    delete ui;
}


//DATA HANDLING FUNCTIONS
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

void MainWindow::recordData()
{
    if (recordingData == false)
    {
        for (QTextBrowser* textBrowser : this->textBrowsers) 
        {
            textBrowser->setTextColor("green");
        }
        ui->recordDataButton->setPalette(QPalette(Qt::darkRed));
        ui->recordDataButton->setText("Stop");
        recordingData = true;
    }
    else
    {
        for (QTextBrowser* textBrowser : this->textBrowsers) 
        {
            textBrowser->setTextColor("white");
        }
        ui->recordDataButton->setPalette(QPalette(Qt::darkGreen));
        ui->recordDataButton->setText("Nagrywaj");
        recordingData = false;
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


