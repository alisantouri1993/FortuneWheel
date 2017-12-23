#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDir"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    createObjects();
    connectObjects();
}

void MainWindow::init()
{
    stringListModel = new QStringListModel(this);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::createObjects()
{

}

void MainWindow::connectObjects()
{
    connect(this,&MainWindow::startStopWheel,ui->widgetWheel,&LuckCircle::startStopWheel);
    connect(this,&MainWindow::velocityChanged,ui->widgetWheel,&LuckCircle::velocityChanged);
    connect(ui->widgetWheel,&LuckCircle::changeSliderValue,this,&MainWindow::changeSliderValue);
    connect(this,&MainWindow::filesInfo,ui->widgetWheel,&LuckCircle::repaintBasedOnFiles);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeSliderValue(int value)
{
    ui->horizontalSliderVelocity->setValue(value);
}

void MainWindow::on_pushButton_clicked()
{
#ifdef __linux__
    path = "/home/internet/Projects/Folder";
#elif _WIN32
    path = "E:/Downloads/Video";
#endif
    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    dir.setNameFilters(QStringList()<<"*.mp4"<<"*.TS");
    QStringList entries = dir.entryList();
    int angle = 0;
    for(int i =0 ; i< entries.count();i++)
    {
        map.insert(angle,entries.at(i));
        angle = angle + 360/entries.count();
    }
    emit filesInfo(entries.count());
    stringListModel->setStringList(entries);
    ui->listView->setModel(stringListModel);
}

void MainWindow::on_pushButtonStartStop_clicked(bool checked)
{
    if(checked)
    {
        ui->pushButtonStartStop->setText("Stop");
        emit startStopWheel(true);
    }
    else
    {
        ui->pushButtonStartStop->setText("Start");
        emit startStopWheel(false);
    }
}

void MainWindow::on_horizontalSliderVelocity_valueChanged(int value)
{
    emit velocityChanged(value);
}
