
#ifndef LOGISTICSCOURSE_H
#define LOGISTICSCOURSE_H


#include <QLinkedList>
#include <QString>
#include <QLinkedList>
#include <QTableWidget>

class LogisticsCourse
{

public:
    LogisticsCourse();
    ~LogisticsCourse();
   int  getTotalNumberOfRobots();
   void setRobotName(QString robotName);
   void setTagAsStationName(QString tagName,QString stationName);
   QString getStationNameAsTag(QString tagName);
   int getStationNameAsIndex(QString tagName);
   QString getStationNameFromIndex(int index);
   QString getRobotName(int robotNumber);
   int getTotalNumberOfStations();

private:
     QList<QString> m_robotLists;
     QList<QString> m_stationNameLists;
     QList<QString> m_tagNameLists;
};

#endif // LOGISTICSCOURSE_H
