#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDir"
#include "QKeyEvent"
#include "QDebug"
#include <QFileDialog>
#include <QCollator>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    init();
    loadSettings();
    createObjects();
    connectObjects();
    labelTimer->start();

}

void MainWindow::init()
{
    stringListModel = new QStringListModel(this);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    attemptsNumber = 1;
}

void MainWindow::createObjects()
{
    movieProcess = new QProcess(this);

    labelTimer   = new QTimer(this);
    labelTimer->setInterval(1500);
}

void MainWindow::connectObjects()
{
    connect(this,&MainWindow::startStopWheel,ui->widgetWheel,&LuckCircle::startStopWheel);
    connect(this,&MainWindow::velocityChanged,ui->widgetWheel,&LuckCircle::velocityChanged);
    connect(ui->widgetWheel,&LuckCircle::changeSliderValue,this,&MainWindow::changeSliderValue);
    connect(this,&MainWindow::filesInfo,ui->widgetWheel,&LuckCircle::repaintBasedOnFiles);
    connect(ui->widgetWheel,&LuckCircle::enableControls,this,&MainWindow::enableControls);
    connect(ui->widgetWheel,&LuckCircle::finalAngle,this,&MainWindow::finalAngle);
    connect(labelTimer,&QTimer::timeout,this,&MainWindow::updateLabel);
}

void MainWindow::saveSettings()
{
    QSettings settings("Ali","Fortune Wheel");

    settings.setValue("Attempts",attemptsNumber);
    settings.setValue("Date",date.currentDate());
}

void MainWindow::loadSettings()
{
    QSettings settings("Ali","Fortune Wheel");

    if(!settings.value("Attempts").isNull())
        attemptsNumber = settings.value("Attempts").toInt();

    if(!settings.value("Date").isNull())
    {
        QDate savedDate = settings.value("Date").toDate();
        if(date.currentDate() != savedDate)
            attemptsNumber = attemptsNumber + savedDate.daysTo(date.currentDate());
    }

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
    if(!map.isEmpty() && !map[rotationAngle].isEmpty())
    {
        QString openedFile =path +"\\"+map[rotationAngle];
        movieProcess->start("C:/KMPlayer/KMPlayer.exe",QStringList()<<openedFile);
    }
}

void MainWindow::updateLabel()
{
    ui->labelTodayAttempts->setText("Today : "+(date.currentDate().toString()+" , Number of Remaining Attempts: "+
                                               QString::number(attemptsNumber)));
    if(!ui->labelTodayAttempts->isHidden())
        ui->labelTodayAttempts->setHidden(true);
    else
        ui->labelTodayAttempts->setHidden(false);

    if(attemptsNumber > 0)
        ui->listView->setDisabled(false);
    else
        ui->listView->setDisabled(true);
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
                attemptsNumber = attemptsNumber - 1;
            }
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();
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
    if(attemptsNumber > 0)
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
            attemptsNumber = attemptsNumber - 1;
        }
    }
    else
    {
        QMessageBox::information(this,"Access Denied !","Your Attmepts have been reached to the Max!");
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
