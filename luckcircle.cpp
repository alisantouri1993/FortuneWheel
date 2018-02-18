#include "luckcircle.h"
#include <QtWidgets>

LuckCircle::LuckCircle(QWidget *parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,&LuckCircle::updateHandler);

    controlTimer = new QTimer(this);
    controlTimer->setInterval(2300);
    connect(controlTimer,&QTimer::timeout,this ,&LuckCircle::reduceSpeed);

    //timer->setInterval(1000);
    //timer->start();

    rotationAngle = 0;

    resize(200,200);
}
///TEST-Test+test
void LuckCircle::updateHandler()
{
    if(rotationAngle == 360)
    {
        rotationAngle = 360/filesCount;
    }
    else
        rotationAngle = rotationAngle + (360/filesCount);
    update();
}

void LuckCircle::reduceSpeed()
{
    qsrand(qrand());
    timer->setInterval(timer->interval() * (fmod(double(qrand()),1.1)+1));
    if(timer->interval() >= 3000)
    {
        emit changeSliderValue(1);
        enableControls();
        timer->stop();
        controlTimer->stop();
        emit finalAngle(rotationAngle);
    }
}

void LuckCircle::startStopWheel(bool state)
{
    if(state)
    {
        timer->start();
        timer->setInterval(1000);
    }
    else
    {
        controlTimer->start();
    }
}

void LuckCircle::velocityChanged(int value)
{
    timer->setInterval(1000/value);
}

void LuckCircle::repaintBasedOnFiles(int value)
{
    filesCount = value;
    rotationAngle = 0;
    update();
}

void LuckCircle::paintEvent(QPaintEvent *)
{
    static const QPoint handler[3] =
    {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -75)
    };
    QColor handlerColor(100, 50, 127);
    QColor choicesColor(0, 127, 127, 191);

    QPainter painter(this);

    int side = qMin(width(), height());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/2,height()/2);
    painter.scale(side / 200, side / 200);

    painter.setPen(Qt::NoPen);
    painter.setBrush(handlerColor);

    painter.save();
    painter.rotate(rotationAngle);
    painter.drawConvexPolygon(handler,3);
    painter.restore();

    painter.setPen(choicesColor);
    painter.setFont(QFont("Arial", 5));
    painter.rotate(-90);

    if(filesCount > 90 )
        painter.setFont(QFont("Arial", 3));
    for (int j = 0; j < filesCount; ++j)
    {
        painter.drawLine(83, 0, 91,0);
        painter.drawText(93,0,QString::number(j+1,10));
        painter.rotate(360/filesCount);
    }
}
