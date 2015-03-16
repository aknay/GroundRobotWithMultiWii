#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo> //need it for adding availabe serial ports
#include <QtSerialPort/QSerialPort>
#include <MavLinkProtocol.h>
#include <Driver/SerialPortLink.h>
#include <QLinkedList>
#include <QTableWidget>
#include "LogisticsCourse.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_sendData_clicked();
    void on_pushButton_Disconnect_clicked();
    void on_pushButton_Connect_clicked();
    void updateOnTable();

private:
    //understanding between stack and heap
    //'MAVLinkProtocol mavlink' vs 'MAVLinkProtocol *mavlink'
    //http://gribblelab.org/CBootcamp/7_Memory_Stack_vs_Heap.html

    Ui::MainWindow *ui;
    QSerialPort *serial; //need it to set serial port setting
    SerialPortLink serialPortLink;
    QList<QString> robot9StationLists;
    QList<QString> allStationLists;
    int stationIdForLable;
    // MAVLinkProtocol mavlink; //for protocol level
    QString currentPortName;
    void setStationLable(int stationNumber ,QString stationName);
    void setStaionName(QString stationName);
    void setTimeStampLabelAccordingToStationAndRobot (QString timeStamp, int stationIndexNumber,int robotID);
    void updateOnTableWidget(QString itemName, int row, int column);

    LogisticsCourse courseOne;
};

#endif // MAINWINDOW_H
