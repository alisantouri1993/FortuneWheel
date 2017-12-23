#ifndef LUCKCIRCLE_H
#define LUCKCIRCLE_H

#include <QWidget>

class LuckCircle : public QWidget
{
    Q_OBJECT
public:
    explicit LuckCircle(QWidget *parent = 0);

public slots:
    void updateHandler();
    void startStopWheel(bool state);
    void velocityChanged(int value);
    void repaintBasedOnFiles(int value);
signals:
    void changeSliderValue(int value);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    QTimer *timer;
    double rotationAngle;

    int filesCount = 60;
    QStringList files;
};

#endif // LUCKCIRCLE_H
