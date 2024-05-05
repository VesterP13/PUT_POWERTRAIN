// Author: Wiktor Preuss 2024
// graphic speedometer
#ifndef SPEEDMETER_H
#define SPEEDMETER_H

#include "qpropertyanimation.h"
#include "qtimer.h"
#include <QWidget>
#include <QLabel>

class SpeedMeter : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedMeter(QWidget *parent = nullptr);
    void update(int rpmValue, int currentValue);
    void simulation(int value, int timeStep);
    void setRpmArcLength(double percentage);
    void setCurrentArcLength(double percentage);
    QFont label_font = QFont("LCD", 22);
    QFont label_font_static = QFont("LCD", 14);
private:
    void paintEvent(QPaintEvent *event);
    void graphicCalculating();
    void startAnimation();
    void labelSetup(QFont label_font, QFont label_font_static);
    void rpm_labelSetup(QFont label_font, QFont label_font_static);
    void current_labelSetup(QFont label_font, QFont label_font_static);
    void onAnimationValueChanged1(const QVariant &value);
    void onAnimationValueChanged2(const QVariant &value);
    void increaseValues();

    QTimer *ptimer;
    QPropertyAnimation *animation = nullptr;
    QPropertyAnimation *animation2 = nullptr;
    QLabel *LabelRPM;
    QLabel *LabelRPM_static;
    QLabel *LabelCURRENT;
    QLabel *LabelCURRENT_static;
    QRectF rectangle1;
    QRectF rectangle2;
    QRectF rectangle3;
    double maxArcLength = 133.0 * 16 * -1;
    double maxCurrentArcLength = 69.0 * 16 * -1;
    int currentRpm;
    int currentCurrent;
    int cricling;
    int arcWidth;
    int arcHeight;
    int arcX;
    int arcY;
    int sizeDiff;
    int whiteArcWidth;
    int whiteArcHeight;
    int animationDuration = 100;
    int x = 0;
    int criclingStep = 0;
    double i = 0;
    double z = 0;

signals:
};

#endif // SPEEDMETER_H
