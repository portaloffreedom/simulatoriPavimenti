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
    bool parse();
    
signals:
    void error(QString);
    void finished(Map*);
    
    
    void beginBorder(const QString description, const QString id);
    void addBorderPolygon();
    void addBorderPolygonPoint(QPointF point);
    void addBorderCircle(QPointF center, qreal radius);
    void closeBorder();
    
    void beginEntrance(const QString description, const QString id);
    void addEntrancePolygon();
    void addEntrancePolygonPoint(QPointF point);
    void addEntranceCircle(QPointF center, qreal radius);
    void closeEntrace();
    
    void beginExit(const QString description, const QString id);
    void addExitPolygon();
    void addExitPolygonPoint(QPointF point);
    void addExitCircle(QPointF center, qreal radius);
    void closeExit();
    
    void beginObstacle(const QString description, const QString id);
    void addObstaclePolygon();
    void addObstaclePolygonPoint(QPointF point);
    void addObstacleCircle(QPointF center, qreal radius);
    void closeObstacle();
    
private:
    bool parsePolygon(QXmlStreamReader &xml, void (MapReader::*addPolygon)(), void (MapReader::*addPolygonPoint)(QPointF));
    
    Map *map;
    QFile *xmlFile;
    bool parseStatus;
};

#endif // MAPREADER_H
