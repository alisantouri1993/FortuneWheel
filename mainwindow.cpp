#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDir"
#include "QKeyEvent"
#include "QDebug"
#include <QFileDialog>
#include <QCollator>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
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
    movieProcess = new QProcess(this);
}

void MainWindow::connectObjects()
{
    connect(this,&MainWindow::startStopWheel,ui->widgetWheel,&LuckCircle::startStopWheel);
    connect(this,&MainWindow::velocityChanged,ui->widgetWheel,&LuckCircle::velocityChanged);
    connect(ui->widgetWheel,&LuckCircle::changeSliderValue,this,&MainWindow::changeSliderValue);
    connect(this,&MainWindow::filesInfo,ui->widgetWheel,&LuckCircle::repaintBasedOnFiles);
    connect(ui->widgetWheel,&LuckCircle::enableControls,this,&MainWindow::enableControls);
    connect(ui->widgetWheel,&LuckCircle::finalAngle,this,&MainWindow::finalAngle);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeSliderValue(int value)
{
    ui->horizontalSliderVelocity->setValue(value);
}

void MainWindow::enableControls()
{
    ui->pushButtonStartStop->setEnabled(true);
    ui->horizontalSliderVelocity->setEnabled(true);
}

void MainWindow::finalAngle(double rotationAngle)
{
    if(!map.isEmpty())
    {
        QString openedFile =path +"\\"+map[rotationAngle];
        movieProcess->start("C:/KMPlayer/KMPlayer.exe",QStringList()<<openedFile);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->text() == "s" || keyEvent->text() == "S")
        {
            if(ui->pushButtonStartStop->isChecked())
            {
                ui->pushButtonStartStop->setText("Start");
                ui->pushButtonStartStop->setDisabled(true);
                ui->horizontalSliderVelocity->setDisabled(true);
                ui->pushButtonStartStop->setChecked(false);
                emit startStopWheel(false);
            }
        }
    }
    return false;
}

void MainWindow::on_pushButton_clicked()
{
    path = QFileDialog::getExistingDirectory(this,"Select The Folder ...",QDir::currentPath());
    ui->lineEditFolderAdd->setText(path);
    //#ifdef __linux__
    //    path = "/home/internet/Projects/Folder";
    //#elif _WIN32
    //    //path = "E:/Downloads/Video";
    //#endif
    if(!path.isEmpty())
    {
        QDir dir(path);
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        dir.setSorting(QDir::NoSort);
        dir.setNameFilters(QStringList()<<"*.mp4");
        QStringList entries = dir.entryList();

        QCollator collator;
        collator.setNumericMode(true);

        std::sort(
                    entries.begin(),
                    entries.end(),
                    [&collator](const QString &name1, const QString &name2)
        {
            return collator.compare(name1, name2) < 0;
        });

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
        ui->pushButtonStartStop->setDisabled(true);
        ui->horizontalSliderVelocity->setDisabled(true);
        emit startStopWheel(false);
    }
}

void MainWindow::on_horizontalSliderVelocity_valueChanged(int value)
{
    emit velocityChanged(value);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString openedFile =path +"\\"+index.data().toString();
    movieProcess->start("C:/KMPlayer/KMPlayer.exe",QStringList()<<openedFile);
}
