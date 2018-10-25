#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSensor>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_clearButton_clicked();
private:
    Ui::MainWindow *ui;
    void initSensors();
    QMap<QString,QSensor *> sensorList;
    QSensor *currentSelectedSensor(QListWidgetItem *select);
private slots:
    void sensorReadingChanged();

};

#endif // MAINWINDOW_H
