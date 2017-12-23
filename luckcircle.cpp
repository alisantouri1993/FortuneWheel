#include "luckcircle.h"
#include <QtWidgets>

LuckCircle::LuckCircle(QWidget *parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,&LuckCircle::updateHandler);

    //timer->setInterval(1000);
    //timer->start();

    rotationAngle = 0;

    resize(200,200);
}

void LuckCircle::updateHandler()
{
    if(rotationAngle == 360)
    {
        rotationAngle = 0;
    }
    else
        rotationAngle = rotationAngle + (6.0);
    update();
}

void LuckCircle::startStopWheel(bool state)
{
    if(state)
    {
        timer->start();
        timer->setInterval(1000);
        emit changeSliderValue(1);
    }
    else
    {
        timer->stop();
        emit changeSliderValue(0);
    }
}

void LuckCircle::velocityChanged(int value)
{
    if(value == 0)
        timer->stop();
    else
        timer->setInterval(1000/value);
}

void LuckCircle::paintEvent(QPaintEvent *)
{
    static const QPoint handler[3] =
    {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -80)
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

    for (int j = 0; j < 60; ++j)
    {
        painter.drawLine(88, 0, 96,0);
        painter.drawText(100,0,QString::number(j+1,10));
        painter.rotate(6.0);
    }

}
