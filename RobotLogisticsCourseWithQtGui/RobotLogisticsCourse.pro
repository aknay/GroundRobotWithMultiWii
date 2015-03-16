#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T18:05:45
#
#-------------------------------------------------

QT       += core gui \
            serialport   #need it for using serial port

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NGControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Driver/SerialPortLink.cpp \
    LogisticsCourse.cpp

HEADERS  += mainwindow.h \
    Driver/SerialPortLink.h \
    LogisticsCourse.h

FORMS    += mainwindow.ui

#to use mavlink in the project; as external library
INCLUDEPATH += libs/mavlink/include/mavlink/v1.0/common
