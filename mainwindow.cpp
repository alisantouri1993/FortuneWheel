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
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
#ifdef __linux__
    path = "/home/internet/Projects/Folder";
#elif _WIN32

#endif
    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    dir.setNameFilters(QStringList()<<"*.pdf");
    QStringList entries = dir.entryList();
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
