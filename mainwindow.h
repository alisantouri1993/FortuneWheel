#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStringListModel>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QProcess>
#include <QSettings>
#include <QDate>

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
    void enableControls();
    void finalAngle(double rotationAngle);
    void updateLabel();
signals:
    void startStopWheel(bool state);
    void velocityChanged(int value);
    void filesInfo(int count);
protected:
    bool eventFilter(QObject* obj, QEvent* event);
    void closeEvent(QCloseEvent *) override;
private slots:
    void on_pushButton_clicked();
    void on_pushButtonStartStop_clicked(bool checked);   
    void on_horizontalSliderVelocity_valueChanged(int value);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    void init();
    void createObjects();
    void connectObjects();

    void saveSettings();
    void loadSettings();
private:
    Ui::MainWindow *ui;
    QString path ;
    QStringListModel * stringListModel;

    QGraphicsScene *graphicScene;
    QGraphicsItem  *graphicsItem;
    QMap<int,QString> map;
    QProcess* movieProcess;

    QDate date;
    int attemptsNumber;
    QTimer *labelTimer;
};

#endif // MAINWINDOW_H
