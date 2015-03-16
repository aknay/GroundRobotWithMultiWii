#include "SerialPortLink.h"


QT_USE_NAMESPACE

SerialPortLink::SerialPortLink( QObject *parent)
    : QObject(parent)
{

    m_serialPort.setBaudRate(QSerialPort::Baud9600); //default baud rate for mavlink

    /*
     [#Explanation]:'readyRead' is signal auto-genetated by QSerialPort whenever it receives data
     when data is received (which is same as signal is activated), handlerReadyRead is called.
      */

    QObject::connect(&m_serialPort, &QSerialPort::readyRead,
                     this, &SerialPortLink::handleReadyRead);
}


QString SerialPortLink::getRobotID(){
    return robotID;
}

QString SerialPortLink::getStationID(){
    return stationID;
}

QString SerialPortLink::getTimeStamp(){
    return timeStamp;
}

void SerialPortLink::handleReadyRead()
{
    QByteArray data = m_serialPort.readAll();

    if (data[0] =='G'){
        isOKToSet = true;
        bufferCount = 0;
    }


    if (isOKToSet){
        Buffer.append(data);
        bufferCount++;
    }

    if (bufferCount>31){
        robotIDandStationID = Buffer;
        Buffer.clear();
        //update time elapsed
        timeElapsed = (timer.elapsed()/1000);

    }


    if ((timeElapsed>=timeDelayForEachReadinSec)){
        isItJustStarted = false;
        timer.restart();
        timeElapsed = 0;

        if (robotIDandStationID.size()==32){
            robotID = robotIDandStationID.at(5);

            QString temp;
            for(int i=18;i<30;i++){
                stationID = temp.append(robotIDandStationID.at(i));
            }
            qDebug()<<"robot ID is";
            qDebug()<<robotID;

            qDebug()<<"station ID is";
            qDebug()<<stationID;

            QTime time = QTime::currentTime();
            QString timeString = time.toString();
            timeStamp = timeString;
            qDebug()<<timeString;

            emit changeInID();
        }
    }
}

SerialPortLink::~SerialPortLink()
{
}

void SerialPortLink::setCurrentPortName(QString currentPortName) {
    m_serialPort.setPortName(currentPortName);
    m_serialPort.open(QIODevice::ReadWrite);
    qDebug()<<currentPortName;
    timer.start();
    isItJustStarted=true;
}

bool SerialPortLink::isConnected() {
    return (m_serialPort.open(QIODevice::ReadWrite) == true) ? true : false; //check whether is connected or not
}

void SerialPortLink::writeBytes(const char* data,qint64 size) {
    m_serialPort.open(QIODevice::ReadWrite); //[#strange]: need this statement to open everytime.

    /*
[#Explanation]:write data to serial port
[#Ref]: from QGroundControl
 */

    QByteArray byteArray(data, size);
    //append data into serialBuffer
    m_serialBuffer.append(byteArray);

    int numWritten = m_serialPort.write(m_serialBuffer);

    //once all data is written, flush the buffer
    m_serialBuffer = m_serialBuffer.remove(0, numWritten);
}








