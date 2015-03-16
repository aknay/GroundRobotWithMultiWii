#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //ui->serialPortComboBox->addItem("hello"); //simple example to item to comboxBox

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->serialPortComboBox->addItem(info.portName());//adding all avaiable ports to comboBox

    ui->pushButton_sendData->setEnabled(false);
    //Example: only checkable button emit (bool checked) // ui->pushButton->setCheckable(true);

    QObject::connect(&serialPortLink, &SerialPortLink::changeInID,this, &MainWindow::updateOnTable);

    //number of robots will be updated automatically. Just keep adding in sequence
    courseOne.setRobotName("Robot One");
    courseOne.setRobotName("Robot Two");
    courseOne.setRobotName("Robot Three");
    courseOne.setRobotName("Robot Four");
     //number of stations will be updated automatically. Just keep adding in sequence
    courseOne.setTagAsStationName("6F005CA80992","St1 - Pick-up");
    courseOne.setTagAsStationName("6A003E39CAA7","St2 - Delivery");
    courseOne.setTagAsStationName("6F005C9E43EE","St3 - Pick-up");
    courseOne.setTagAsStationName("6F005CADAC32","St4 - Delivery");
    courseOne.setTagAsStationName("6F005C64DD8A","St5 - Pick-up");
    courseOne.setTagAsStationName("6F005C941CBB","St6 - Delivery");
    courseOne.setTagAsStationName("6F005CA5CA5C","St7 - Pick-up");
    courseOne.setTagAsStationName("6F005C6B6139","St8 - Delivery");
    courseOne.setTagAsStationName("6F005C867CC9","St9 - Pick-up");
    courseOne.setTagAsStationName("6F005CAC0F90","St10 - Delivery");
    courseOne.setTagAsStationName("6F005C899D27","St11 - Delivery");
    courseOne.setTagAsStationName("6F005C843285","St12 - Delivery");

    ui->tableWidget->setRowCount(courseOne.getTotalNumberOfStations());
    ui->tableWidget->setColumnCount(courseOne.getTotalNumberOfRobots()+1);

    //populate robot name in Horizontal Header
    for (int i=0;i<courseOne.getTotalNumberOfRobots();i++)
    {
        ui->tableWidget->setHorizontalHeaderItem(i+1,new QTableWidgetItem(courseOne.getRobotName(i)));
    }

    //populate station Name in station Column
    for (int i=0;i<courseOne.getTotalNumberOfStations();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(courseOne.getStationNameFromIndex(i)));
    }
}

void MainWindow::updateOnTable(){

    QString stationID = serialPortLink.getStationID();
    QString robotID = serialPortLink.getRobotID();
    QString timeStamp = serialPortLink.getTimeStamp();
    int stationIndexNumber = courseOne.getStationNameAsIndex(stationID);
    int robotIdAsInt = robotID.toInt();

    ui->tableWidget->setItem(stationIndexNumber,robotIdAsInt,new QTableWidgetItem(timeStamp));

}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_sendData_clicked() {

}

void MainWindow::on_pushButton_Disconnect_clicked()
{       //these are to simply lock user from sending data while connection is not estiblished yet
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Disconnect->setEnabled(false);
    ui->serialPortComboBox->setEnabled(true);
    ui->pushButton_sendData->setEnabled(false);
}

void MainWindow::on_pushButton_Connect_clicked()
{
    currentPortName = ui->serialPortComboBox->currentText();

    serialPortLink.setCurrentPortName(currentPortName);

    //       mavlink.setCurrentPortName(currentPortName);
    //these are to simply lock user from sending data while connection is not estiblished yet.
    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_Disconnect->setEnabled(true);
    ui->serialPortComboBox->setEnabled(false);
    ui->pushButton_sendData->setEnabled(true);
}
