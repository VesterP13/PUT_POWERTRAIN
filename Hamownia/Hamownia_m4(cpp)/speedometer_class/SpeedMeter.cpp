// Author: Wiktor Preuss 2024
// graphic speedometer
#include "SpeedMeter.h"
//#include "qmainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QPropertyAnimation>
#include <QLabel>

SpeedMeter::SpeedMeter(QWidget *parent)    : QWidget{parent}
{
    setFixedSize(parent->width()/4, parent->width()/4);
    graphicCalculating();
    labelSetup(label_font, label_font_static);
    parentPalette = parent->palette();
    //setCurrentArcLength(0);    // value in %
    //setRpmArcLength(0);        // value in %
    //simulation(10, 200);       //(Value step, Time Step)
}

void SpeedMeter::labelSetup(QFont label_font, QFont label_font_static){
    rpm_labelSetup(label_font, label_font_static);
    current_labelSetup(label_font, label_font_static);
}

void SpeedMeter::rpm_labelSetup(QFont label_font, QFont label_font_static){
    LabelRPM = new QLabel(this);
    LabelRPM_static = new QLabel(this);

    LabelRPM->setText("00000");
    LabelRPM->setFont(label_font);
    LabelRPM->move(width()/2-width()/6, height() * 55/100);

    LabelRPM_static->setText("rpm");
    LabelRPM_static->setFont(label_font_static);
    LabelRPM_static->move(0, height() * 63/100);
}

void SpeedMeter::current_labelSetup(QFont label_font, QFont label_font_static){
    LabelCURRENT = new QLabel(this);
    LabelCURRENT_static = new QLabel(this);

    LabelCURRENT->setText("10");
    LabelCURRENT->setFont(label_font);
    LabelCURRENT->move(width()/2-width()/6, height() * 65/100);

    LabelCURRENT_static->setText("A");
    LabelCURRENT_static->setFont(label_font_static);
    LabelCURRENT_static->move(40, height() * 63/100);
}

void SpeedMeter::simulation(int value, int timeStep){
    if (value < 0) value = - value;
    criclingStep = value;
    stepValue = value;
    ptimer = new QTimer();
    ptimer->start(timeStep);
    QObject::connect(ptimer, &QTimer::timeout, this, &SpeedMeter::increaseValues);
}

void SpeedMeter::increaseValues(){
    if (x>=100){
        x = x - 100;
        cricling = cricling + criclingStep*16;
    }
    else x = x + stepValue;
    cricling = cricling + criclingStep*16;
    this->setRpmArcLength(x);
    this->setCurrentArcLength(x*2/3);
    repaint();
}

void SpeedMeter::graphicCalculating(){
    int width = this->width();
    int height = this->height();

    arcWidth = width - 50;
    arcHeight = height - 50;
    arcX = (width - arcWidth) / 2;
    arcY = height / 4;
    rectangle1.setRect(arcX, arcY, arcWidth, arcHeight);
    sizeDiff = 20;
    rectangle3.setRect(arcX+sizeDiff, arcY+sizeDiff, arcWidth-(2*sizeDiff), arcHeight-(2*sizeDiff));
    sizeDiff = 10;
    whiteArcWidth = arcWidth-(2*sizeDiff);
    whiteArcHeight = arcHeight-(2*sizeDiff);
    rectangle2.setRect(arcX+sizeDiff, arcY+sizeDiff, whiteArcWidth, whiteArcHeight);

    animation = new QPropertyAnimation(this, "i");
    animation->setDuration(animationDuration);
    animation->setEasingCurve(QEasingCurve::Linear);
    connect(animation, &QPropertyAnimation::valueChanged, this, &SpeedMeter::onAnimationValueChanged1);

    animation2 = new QPropertyAnimation(this, "z");
    animation2->setDuration(animationDuration);
    animation2->setEasingCurve(QEasingCurve::Linear);
    connect(animation2, &QPropertyAnimation::valueChanged, this, &SpeedMeter::onAnimationValueChanged2);
}

void SpeedMeter::update(int rpmValue, int currentValue) {
    setRpmArcLength(rpmValue);
    setCurrentArcLength(currentValue);
}

void SpeedMeter::paintEvent(QPaintEvent *event)
{

    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient1(rectangle1.topLeft(), rectangle1.topRight());
    gradient1.setColorAt(0, QColorConstants::Svg::orange);
    gradient1.setColorAt(0.4, QColorConstants::Svg::red);
    painter.setPen(QPen(gradient1, 20));
    painter.drawArc(rectangle1, 175*16, i);

    QLinearGradient gradient2(rectangle1.topLeft(), rectangle1.topRight());
    gradient2.setColorAt(0, QColorConstants::Svg::blue);
    gradient2.setColorAt(0.4, QColorConstants::Svg::darkblue);
    painter.setPen(QPen(gradient2, 15));
    painter.drawArc(rectangle3, 175.5*16, z);

    painter.setPen(QPen(Qt::white, 2));
    int whiteArcAngle = - 140;
    painter.drawArc(rectangle2, 240*16, -200*16);

    painter.setPen(QPen(Qt::white, 2));
    int numLines = 20;
    int angleStep = whiteArcAngle / numLines;
    int startAngle = 180;

    for (int i = 0; i <= numLines; ++i)
    {
        if (i % 5 ==0){
            int currentAngle1 = startAngle + i * angleStep;
            QPointF startPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2) * qSin(qDegreesToRadians(currentAngle1)));
            QPointF endPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2 + 25) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2 + 25) * qSin(qDegreesToRadians(currentAngle1)));
            painter.drawLine(startPoint1, endPoint1);
        }
        else {
            int currentAngle1 = startAngle + i * angleStep;
            QPointF startPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2) * qSin(qDegreesToRadians(currentAngle1)));
            QPointF endPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2 + 10) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2 + 10) * qSin(qDegreesToRadians(currentAngle1)));
            painter.drawLine(startPoint1, endPoint1);
        }
    }

    numLines = 20;
    angleStep = whiteArcAngle * 2/3 / numLines;
    startAngle = 180;

    for (int i = 0; i <= numLines; ++i)
    {
        if (i % 5 ==0){
            int currentAngle1 = startAngle + i * angleStep;
            QPointF startPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2) * qSin(qDegreesToRadians(currentAngle1)));
            QPointF endPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2 - 15) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2 - 15) * qSin(qDegreesToRadians(currentAngle1)));
            painter.drawLine(startPoint1, endPoint1);
        }
        else {
            int currentAngle1 = startAngle + i * angleStep;
            QPointF startPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2) * qSin(qDegreesToRadians(currentAngle1)));
            QPointF endPoint1 = rectangle2.center() + QPointF((whiteArcWidth / 2 - 5) * qCos(qDegreesToRadians(currentAngle1)), -(whiteArcHeight / 2 - 5) * qSin(qDegreesToRadians(currentAngle1)));
            painter.drawLine(startPoint1, endPoint1);
        }

    }
    sizeDiff = 40;
    QRectF rectangle4(arcX+sizeDiff, arcY+sizeDiff, arcWidth-(2*sizeDiff), arcHeight-(2*sizeDiff));
    QLinearGradient gradient3(rectangle1.topLeft(), rectangle1.topRight());
    gradient2.setColorAt(0, QColorConstants::Svg::white);
    gradient2.setColorAt(0.4, QColorConstants::Svg::white);
    painter.setPen(QPen(gradient2, 10));
    painter.drawArc(rectangle4, cricling, 140*16);
    painter.setPen(QPen(gradient2, 10));
    painter.drawArc(rectangle4, cricling+180*16, 140*16);
}

void SpeedMeter::onAnimationValueChanged1(const QVariant &value) {
    i = value.toDouble();
    repaint();
}

void SpeedMeter::onAnimationValueChanged2(const QVariant &value) {
    z = value.toDouble();
    repaint();
}

void SpeedMeter::setRpmArcLength(double percentage)
{
    if (percentage > 100) {
        percentage = 100;
    }

    double newArcLength1 = static_cast<double>(percentage / 100.0 * maxArcLength);

    if (newArcLength1 != i) {
        int newRpm = static_cast<int>(percentage);
        if (newRpm != currentRpm) {
            currentRpm = newRpm;
            LabelRPM->setText(QString::number(currentRpm));
        }

        if (!(animation->state() == QAbstractAnimation::Running)) {
            int startValue = i;
            int endValue = newArcLength1;

            animation->setStartValue(startValue);
            animation->setEndValue(endValue);
            animation->start();
        }
    }
}

void SpeedMeter::setCurrentArcLength(double percentage)
{
    if (percentage > 100)
    {
        percentage = 100;
    }

    double newArcLength = static_cast<double>(percentage / 100.0 * maxCurrentArcLength);

    if (newArcLength != z)
    {
        int newCurrent = static_cast<int>(percentage);
        if (newCurrent != currentCurrent)
        {
            currentCurrent = newCurrent;
            LabelCURRENT->setText(QString::number(currentCurrent));
        }

        if (!(animation2->state() == QAbstractAnimation::Running))
        {
            int startValue = z;
            int endValue = newArcLength;

            animation2->setStartValue(startValue);
            animation2->setEndValue(endValue);
            animation2->start();
        }
    }
}

