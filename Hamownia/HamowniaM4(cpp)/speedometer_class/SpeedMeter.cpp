#include "SpeedMeter.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QPropertyAnimation>
#include <QLabel>


SpeedMeter::SpeedMeter(QWidget *parent)    : QWidget{parent}
{
    setFixedSize(250,250);
    graphicCalculating();

    LabelRPM = new QLabel(this);
    LabelRPM_static = new QLabel(this);

    QFont font("LCD", 22);
    QFont font_static("LCD", 14);

    LabelRPM->setText("30 000");
    LabelRPM->setFont(font);
    LabelRPM->move(width()/2-width()/6, height() * 55/100);

    LabelRPM_static->setText("rpm");
    LabelRPM_static->setFont(font_static);
    LabelRPM_static->move(0, height() * 63/100);


    LabelCURRENT = new QLabel(this);
    LabelCURRENT_static = new QLabel(this);

    LabelCURRENT->setText("10");
    LabelCURRENT->setFont(font);
    LabelCURRENT->move(width()/2-width()/6, height() * 65/100);

    LabelCURRENT_static->setText("A");
    LabelCURRENT_static->setFont(font_static);
    LabelCURRENT_static->move(40, height() * 63/100);
}

void SpeedMeter::simulation(int value){
    criclingStep = value;
    ptimer = new QTimer();
    ptimer->start(500);
    QObject::connect(ptimer, &QTimer::timeout, this, &SpeedMeter::increaseByTen);
}

void SpeedMeter::increaseByTen(){
    if (x>=100){
        x = x - 100;
        cricling = cricling + criclingStep*16;
    }
    else x = x + 1;
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

}


void SpeedMeter::paintEvent(QPaintEvent *event)
{

    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //RPM ARC
    //QRectF rectangle1(arcX, arcY, arcWidth, arcHeight);
    QLinearGradient gradient1(rectangle1.topLeft(), rectangle1.topRight());
    gradient1.setColorAt(0, QColorConstants::Svg::orange);
    gradient1.setColorAt(0.4, QColorConstants::Svg::red);
    painter.setPen(QPen(gradient1, 20));
    painter.drawArc(rectangle1, 177*16, i);

    //CURRENT ARC
    //int sizeDiff = 20;
    //QRectF rectangle3(arcX+sizeDiff, arcY+sizeDiff, arcWidth-(2*sizeDiff), arcHeight-(2*sizeDiff));
    QLinearGradient gradient2(rectangle1.topLeft(), rectangle1.topRight());
    gradient2.setColorAt(0, QColorConstants::Svg::blue);
    gradient2.setColorAt(0.4, QColorConstants::Svg::darkblue);
    painter.setPen(QPen(gradient2, 15));
    painter.drawArc(rectangle3, 175.5*16, z);

    //WHITE ARC + CUTTING ARCS
    //QColor backgroundColor = palette().color(QPalette::Window);
    //QColor backgroundColor = QColorConstants::Svg::red;
    //sizeDiff = 10;
    //int whiteArcWidth = arcWidth-(2*sizeDiff);
    //int whiteArcHeight = arcHeight-(2*sizeDiff);
    //QRectF rectangle2(arcX+sizeDiff, arcY+sizeDiff, whiteArcWidth, whiteArcHeight);

    QColor backgroundColor = palette().color(QPalette::Base);
    painter.setPen(QPen(backgroundColor, 10));
    QPointF startPoint1 = rectangle2.center() + QPointF(200, 5.5);
    QPointF endPoint1 = rectangle2.center() + QPointF(-200, 5.5);
    painter.drawLine(startPoint1, endPoint1);

    painter.setPen(QPen(Qt::white, 2));
    int whiteArcAngle = - 140;
    painter.drawArc(rectangle2, 240*16, -200*16);

    // METRICS
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

    //CIRCLE
    sizeDiff = 40;
    QRectF rectangle4(arcX+sizeDiff, arcY+sizeDiff, arcWidth-(2*sizeDiff), arcHeight-(2*sizeDiff));
    QLinearGradient gradient3(rectangle1.topLeft(), rectangle1.topRight());
    gradient2.setColorAt(0, QColorConstants::Svg::blue);
    gradient2.setColorAt(0.4, QColorConstants::Svg::darkblue);
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

void SpeedMeter::setRpmArcLength(double percentage) {

    if (percentage > 100){
        percentage = 100;
    }

    double newArcLength = static_cast<double>(percentage / 100.0 * 133.0 * 16 * -1);

    //TUTAJ ZMIANA WYSWIETLANEGO RPM
    this->LabelRPM->setText(QString::number(x*212));

    if (!animation) {
        animation = new QPropertyAnimation(this, "i");
        animation->setDuration(animationDuration); // Czas trwania animacji w milisekundach
        animation->setEasingCurve(QEasingCurve::Linear); // Ustawienie równomiernego przebiegu animacji
        connect(animation, &QPropertyAnimation::valueChanged, this, &SpeedMeter::onAnimationValueChanged1);
    }

    int startValue = i;
    int endValue = newArcLength;

    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->start();

}

void SpeedMeter::setCurrentArcLength(double percentage) {

    if (percentage > 100){
        percentage = 100;
    }

    double newArcLength = static_cast<double>(percentage / 100.0 * 69.0 * 16 * -1);

    //TUTAJ ZMIANA WYSWIETLANEGO RPM
    this->LabelCURRENT->setText(QString::number(x/7));

    if (!animation2) {
        animation2 = new QPropertyAnimation(this, "z");
        animation2->setDuration(this->animationDuration);
        animation2->setEasingCurve(QEasingCurve::Linear);
        connect(animation2, &QPropertyAnimation::valueChanged, this, &SpeedMeter::onAnimationValueChanged2);
    }

    int startValue = z; // Aktualna wartość długości łuku
    int endValue = newArcLength; // Nowa wartość długości łuku

    animation2->setStartValue(startValue);
    animation2->setEndValue(endValue);
    animation2->start();

}


