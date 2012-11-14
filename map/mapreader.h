/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef MAPREADER_H
#define MAPREADER_H

#include <QtCore/QObject>
#include <QFile>
#include <qxmlstream.h>
#include "map.h"


class MapReader : public QObject
{
Q_OBJECT
public:
    MapReader(QFile& xmlFile);
    virtual ~MapReader();
    bool getMap();
    
signals:
    void error(QString);
    void finished(Map*);
    
    
    void beginBorder();
    void addBorderPoint(QPointF point);
    void closeBorder();
    
    void beginEntrance();
    void addEntrancePoint(QPointF point);
    void closeEntrace();
    
    void beginExit();
    void addExitPoint(QPointF point);
    void closeExit();
    
    void beginObstacle();
    void addObstaclePoint(QPointF point);
    void closeObstacle();
    
private:
    bool parse();
    bool parsePolygon(QXmlStreamReader &xml, void (MapReader::*)(), void (MapReader::*)(QPointF), void (MapReader::*)());
    
    Map *map;
    QFile *xmlFile;
    bool parseStatus;
};

#endif // MAPREADER_H
