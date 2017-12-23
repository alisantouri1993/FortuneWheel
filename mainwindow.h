#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStringListModel>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void startStopWheel(bool state);
    void velocityChanged(int value);
private slots:
    void on_pushButton_clicked();
private slots:
    void on_pushButtonStartStop_clicked(bool checked);   
    void on_horizontalSliderVelocity_valueChanged(int value);

private:
    void init();
    void createObjects();
    void connectObjects();

private:
    Ui::MainWindow *ui;
    QString path ;
    QStringListModel * stringListModel;

    QGraphicsScene *graphicScene;
    QGraphicsItem  *graphicsItem;
};

#endif // MAINWINDOW_H
