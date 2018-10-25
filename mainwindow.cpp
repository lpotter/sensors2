#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSensor>
#include <QSensorManager>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->ensureCursorVisible();
    initSensors();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSensors()
{
    foreach (const QByteArray &type, QSensor::sensorTypes()) {
        foreach (const QByteArray &identifier, QSensor::sensorsForType(type)) {
            // Don't put in sensors we can't connect to
            QSensor* sensor = new QSensor(type, this);
            sensor->setIdentifier(identifier);
            if (!sensor->connectToBackend()) {
                qDebug() << "Couldn't connect to" << identifier;
                continue;
            }

            sensorList.insert(identifier, sensor);

            auto *item = new QListWidgetItem(sensor->type());
            QVariant v;
            v.setValue(identifier);
            item->setData(Qt::UserRole, v);
            ui->listWidget->addItem(item);
        }
    }

    if (sensorList.isEmpty()) {
        ui->textEdit->insertPlainText(QLatin1Literal("No sensors found"));
    }
}

void MainWindow::on_startButton_clicked()
{
    if (ui->listWidget->currentItem()) {
        ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->listWidget->setDisabled(true);
        QSensor *thisSensor = currentSelectedSensor(ui->listWidget->currentItem());
        connect(thisSensor, SIGNAL(readingChanged()),this,SLOT(sensorReadingChanged()));
        thisSensor->start();
    }
}

void MainWindow::on_stopButton_clicked()
{
    if (ui->listWidget->currentItem()) {
        QSensor *thisSensor = currentSelectedSensor(ui->listWidget->currentItem());
        thisSensor->stop();
        disconnect(thisSensor, SIGNAL(readingChanged()),this,SLOT(sensorReadingChanged()));
        ui->listWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        ui->listWidget->setDisabled(false);
    }
}

void MainWindow::sensorReadingChanged()
{
    QSensor *thisSensor = currentSelectedSensor(ui->listWidget->currentItem());

    QSensorReading *reading = thisSensor->reading();
    QString sensorLine;
    sensorLine = thisSensor->type();
    sensorLine += " " + QString::number(reading->timestamp());

    for (int i = 0; i < reading->valueCount(); i++) {
        sensorLine += " " + reading->value(i).toString();
    }
    sensorLine += "\n";
    ui->textEdit->insertPlainText(sensorLine);
    ui->textEdit->moveCursor(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
}

void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

QSensor *MainWindow::currentSelectedSensor(QListWidgetItem *item)
{
    QVariant v = item->data(Qt::UserRole);
    QString id = v.value<QString>();
    return sensorList.value(id);
}
