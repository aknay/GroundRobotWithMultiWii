#include "LogisticsCourse.h"

LogisticsCourse::LogisticsCourse()
{
}

LogisticsCourse::~LogisticsCourse()
{

}

void  LogisticsCourse::setRobotName(QString robotName){
    m_robotLists.append(robotName);
}

QString  LogisticsCourse::getRobotName(int robotNumber){
    return m_robotLists.at(robotNumber);
}

int LogisticsCourse::getTotalNumberOfRobots(){
    return m_robotLists.size();
}

void LogisticsCourse::setTagAsStationName(QString tagName,QString stationName){
    m_stationNameLists.append(stationName);
    m_tagNameLists.append(tagName);
}

QString LogisticsCourse::getStationNameAsTag(QString tagName){
    return  m_tagNameLists.indexOf(tagName) == -1 ?
                "Found None"
              : m_stationNameLists.at(m_tagNameLists.indexOf(tagName));
}

int LogisticsCourse::getStationNameAsIndex(QString tagName){
    return m_tagNameLists.indexOf(tagName);
}


int LogisticsCourse::getTotalNumberOfStations(){
    return m_stationNameLists.size();
}
QString LogisticsCourse::getStationNameFromIndex(int index){
    return m_stationNameLists.at(index);
}




