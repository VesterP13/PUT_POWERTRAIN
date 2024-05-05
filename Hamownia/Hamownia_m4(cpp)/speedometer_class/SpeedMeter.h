// Wiktor Preuss 2024
// for PUT POWERTRAIN
// Alpha bulid

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
    void paintEvent(QPaintEvent *event);

    void graphicCalculating();
    void startAnimation();
    void simulation(int value);
    void increaseByTen();
    void setRpmArcLength(double percentage);
    void setCurrentArcLength(double percentage);

    int animationDuration = 100;
    int x = 0;
    int criclingStep = 0;

    QLabel *LabelRPM;
    QLabel *LabelRPM_static;
    QLabel *LabelCURRENT;
    QLabel *LabelCURRENT_static;

    int cricling;

    int arcWidth;
    int arcHeight;
    int arcX;
    int arcY;
    QRectF rectangle1;
    QRectF rectangle2;
    QRectF rectangle3;
    int sizeDiff;
    int whiteArcWidth;
    int whiteArcHeight;


private:
    double i = 0;     //double i = -110*16;
    double z = -30;

    QTimer *ptimer;

    void onAnimationValueChanged1(const QVariant &value);
    void onAnimationValueChanged2(const QVariant &value);

    QPropertyAnimation *animation = nullptr;
    QPropertyAnimation *animation2 = nullptr;
signals:
};

#endif // SPEEDMETER_H
