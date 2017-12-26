#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStringListModel>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void changeSliderValue(int value);
signals:
    void startStopWheel(bool state);
    void velocityChanged(int value);
    void filesInfo(int count);
protected:
    bool eventFilter(QObject* obj, QEvent* event);
private slots:
    void on_pushButton_clicked();
    void on_pushButtonStartStop_clicked(bool checked);   
    void on_horizontalSliderVelocity_valueChanged(int value);

    void on_listView_doubleClicked(const QModelIndex &index);

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
    QMap<int,QString> map;
    QProcess* movieProcess;
};

#endif // MAINWINDOW_H
