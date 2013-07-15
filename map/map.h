/*
 *  <one line to give the program's name and a brief idea of what it does.>
 *  Copyright (C) 2012  Matteo De Carlo <matteo.dek@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MAP_H
#define MAP_H

#include <QWidget>
//#include <QGLWidget>
#include <QEvent>
#include "figure.h"
#include "spatialobject.h"
#include "../service/simulatorePavimentiTypes.h"
#include "../service/settingswidget.h"
#include "../engine/groundsensor.h"


class Map : public QWidget
{
Q_OBJECT
public:
    Map(SettingsWidget *settingswidget);
    virtual ~Map();

    QPointF getEntrancePoint();
    virtual QSize sizeHint() const;
    QVector<GroundSensor*> &getSensorMap() {
      return this->sensorMap;  
    };
    
    static char version[];

signals:
    void isDrawing(QPainter &painter);
    
public slots:
    void addSensor(GroundSensor *sensor)
        { sensorMap.append(sensor); };
        
    void beginBorder(const QString description, const QString id);
    void addBorderPolygon();
    void addBorderPolygonPoint(QPointF point);
    void addBorderCircle(QPointF center, smReal radius);
    void closeBorder();
    
    void beginEntrance(const QString description, const QString id);
    void addEntrancePolygon();
    void addEntrancePolygonPoint(QPointF point);
    void addEntranceCircle(QPointF center, smReal radius);
    void closeEntrance();
    
    void beginExit(const QString description, const QString id);
    void addExitPolygon();
    void addExitPolygonPoint(QPointF point);
    void addExitCircle(QPointF center, smReal radius);
    void closeExit();
    
    void beginObstacle(const QString description, const QString id);
    void addObstaclePolygon();
    void addObstaclePolygonPoint(QPointF point);
    void addObstacleCircle(QPointF center, smReal radius);
    void closeObstacle();
    
protected:
    void paintEvent(QPaintEvent *event);
    
private:

    SettingsWidget *settingswidget;
    
    smReal   leftConstraint;
    smReal  rightConstraint;
    smReal    topConstraint;
    smReal bottomConstraint;

    QVector<SpatialObject*> borders;
    QVector<SpatialObject*> entrances;
    QVector<SpatialObject*> exits;
    QVector<SpatialObject*> obstacles;
    
    QVector<GroundSensor*> sensorMap;
    void drawSensorMap(QPainter &p);
    
    void analyzeConstraints(const QPointF point);
    void analyzeCirlceConstraints(const QPointF center, const smReal radius);
    
    void beginObject(QVector< SpatialObject* >& objectVect, QString name, QString description, const QString id);
    void addObjectPolygon(QVector< SpatialObject* >& objectVect);
    void addObjectPolygonPoint(QVector< SpatialObject* >& objectVect, QPointF point);
    void addObjectCircle(QVector< SpatialObject* >& objectVect, QPointF center, smReal radius);
    void closeObject(QVector< SpatialObject* >& objectVect);
};

#endif // MAP_H
