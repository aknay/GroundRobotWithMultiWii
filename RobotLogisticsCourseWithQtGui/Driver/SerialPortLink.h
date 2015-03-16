#ifndef SERIALPORTLINK_H
#define SERIALPORTLINK_H

#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QByteArray>
#include <QObject>
#include <QDateTime> //nay


QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE


class SerialPortLink : public QObject
{
    Q_OBJECT

public:
    SerialPortLink(QObject *parent = 0);
    ~SerialPortLink();
    bool isConnected();
    void setCurrentPortName(QString currentPortName);
    void writeBytes(const char* data,qint64 size);
    QString getRobotID();
    QString getStationID();
    QString getTimeStamp();

private slots:
    void handleReadyRead();
signals:
    void changeInID();

private:
    QSerialPort m_serialPort;
    QByteArray m_serialBuffer; // An internal buffer for receiving data from member functions and actually transmitting them via the serial port.
    QByteArray Buffer;
    QByteArray robotIDandStationID;
    QString robotID;
    QString stationID;
    bool isOKToSet;
    int bufferCount;
    QTime timer;
    long timeElapsed;
    bool isItJustStarted;
    QString timeStamp;
    static const int timeDelayForEachReadinSec = 1;
};

#endif // SERIALPORTLINK_H

//[#Lesson Learned]: Both h file and source files must be same in order to work "F4-> Swither Header/Source"
