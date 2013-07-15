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

#include <iostream>
#include <cmath>
#include <QPainter>
#include <QPaintEvent>

#include "map.h"
#include "polygon.h"
#include "circle.h"
#include "../service/randomservice.h"

char Map::version[] = "0.2";

Map::Map(SettingsWidget *settingswidget) 
    :settingswidget(settingswidget)
{   
    //TODO cambiare le costanti con massimo e minimo dei float
    this->leftConstraint = 1.0e70;
    this->rightConstraint = -1.0e70;
    this->topConstraint = -1.0e70;
    this->bottomConstraint = 1.0e70;

    this->setMinimumSize(100,100);
}

QSize Map::sizeHint() const
{
    return QSize(400,400);
}

template <class elimination>
void deleteInsideVector(QVector<elimination*> vector)
{   
    foreach(elimination *obj, vector) {
        delete obj;
    }
}

Map::~Map()
{
    deleteInsideVector<SpatialObject>(borders);
    deleteInsideVector<SpatialObject>(entrances);
    deleteInsideVector<SpatialObject>(exits);
    deleteInsideVector<SpatialObject>(obstacles);
    deleteInsideVector<GroundSensor>(sensorMap);
 }

 
void drawInsidePolygon(QPainter& painter,QVector<SpatialObject*> vector)
{
    for (int i = 0; i < vector.size(); i++) {
	vector[i]->draw(painter);
    }
}

void Map::paintEvent(QPaintEvent* event)
{
    float margin = 10;
    
    QPainter painter;
    painter.begin(this);
    painter.setRenderHints(settingswidget->getRenderingHint());
    painter.save();
    
    //draw ---------------------------------------------------------------------
    
    //draw background
    painter.fillRect(event->rect(),Qt::black);

    //new origin point
    smReal scalex = (rect().width()-margin*2)/(rightConstraint-leftConstraint);
    smReal scaley = ((rect().height()-margin*2)/(topConstraint-bottomConstraint));

    smReal scale = scaley;
    if (scalex < scaley)
	scale = scalex;
    
    smReal diffScaleX = (((rect().width()-margin*2)/scale) -(rightConstraint-leftConstraint))/2;
    smReal diffScaleY = (((rect().height()-margin*2)/scale)-(topConstraint-bottomConstraint))/2;
    
    painter.translate(margin,rect().bottom()-margin);
    painter.scale(scale,-scale);
    painter.translate(diffScaleX-leftConstraint,diffScaleY-bottomConstraint);


    //draw borders
    painter.setPen(QPen(Qt::gray));
    drawInsidePolygon(painter,borders);
    
    //draw exits
    painter.setPen(QPen(Qt::red));
    drawInsidePolygon(painter,exits);
    
    //draw entrances
    painter.setPen(QPen(Qt::green));
    drawInsidePolygon(painter,entrances);
    
    //draw obstacles
    painter.setPen(QPen(Qt::white));
    drawInsidePolygon(painter,obstacles);
    
    drawSensorMap(painter);
    
    emit isDrawing(painter);
    
    //fine disegno -------------------------------------------------------------
    painter.restore();
    painter.end();
    
}
void Map::drawSensorMap ( QPainter& p )
{
    foreach (GroundSensor* sensor, sensorMap) {
        sensor->draw(p);
    }
}


void Map::analyzeCirlceConstraints(const QPointF center, const smReal radius)
{
    QPointF upperRight = center + QPointF(radius,radius);
    QPointF bottomLeft = center - QPointF(radius,radius);
    
    analyzeConstraints(upperRight);
    analyzeConstraints(bottomLeft);
}

void Map::analyzeConstraints(const QPointF point)
{
    if (point.x() < leftConstraint)
	leftConstraint = point.x();
    if (point.x() > rightConstraint)
	rightConstraint = point.x();
    if (point.y() < bottomConstraint)
	bottomConstraint = point.y();
    if (point.y() > topConstraint)
	topConstraint = point.y();
}

void Map::beginObject(QVector< SpatialObject* > &objectVect, QString name, QString description, const QString id)
{
    objectVect.append(new SpatialObject(name,description,id));
}
void Map::addObjectPolygon(QVector< SpatialObject* > &objectVect)
{
    objectVect.last()->addFigure(new Polygon());
}
void Map::addObjectPolygonPoint(QVector< SpatialObject* > &objectVect, QPointF point)
{
    reinterpret_cast<Polygon*>(objectVect.last()->lastFigure())->addVertex(point);
    analyzeConstraints(point);
//     std::cout<<"Aggiunto un punto al bordo: "<<point.x()<<","<<point.y()<<std::endl;
}
void Map::addObjectCircle(QVector< SpatialObject* > &objectVect, QPointF center, smReal radius)
{
    objectVect.last()->addFigure(new Circle(center,radius));
    analyzeCirlceConstraints(center,radius);
}
void Map::closeObject(QVector< SpatialObject* > &objectVect)
{
    //To Optimize Memory
//     objectVect->resize(border_v->size());
}


//Border section
void Map::beginBorder(const QString description, const QString id)
{
    beginObject(borders,"Border",description,id);
}
void Map::addBorderPolygon()
{
    addObjectPolygon(borders);
}
void Map::addBorderPolygonPoint(QPointF point)
{   
    addObjectPolygonPoint(borders,point);
}
void Map::addBorderCircle(QPointF center, smReal radius)
{
    addObjectCircle(borders,center,radius);
}
void Map::closeBorder()
{
    closeObject(borders);
}

//entrance section
void Map::beginEntrance(const QString description, const QString id)
{
    beginObject(entrances,"Entrance",description,id);
}
void Map::addEntrancePolygon()
{
    addObjectPolygon(entrances);
}
void Map::addEntrancePolygonPoint(QPointF point)
{
    addObjectPolygonPoint(entrances,point);
}
void Map::addEntranceCircle(QPointF center, smReal radius)
{
    addObjectCircle(entrances,center,radius);
}
void Map::closeEntrance()
{
    closeObject(entrances);
}

//Exit section
void Map::beginExit(const QString description, const QString id)
{
    beginObject(exits,"Exit",description,id);
}
void Map::addExitPolygon()
{
    addObjectPolygon(exits);
}
void Map::addExitPolygonPoint(QPointF point)
{
    addObjectPolygonPoint(exits,point);
}
void Map::addExitCircle(QPointF center, smReal radius)
{
    addObjectCircle(exits,center,radius);
}
void Map::closeExit()
{
    closeObject(exits);
}

//Obstacle section
void Map::beginObstacle(const QString description, const QString id)
{
    beginObject(obstacles,"Obstacle",description,id);
}
void Map::addObstaclePolygon()
{
    addObjectPolygon(obstacles);
}
void Map::addObstaclePolygonPoint(QPointF point)
{
    addObjectPolygonPoint(obstacles,point);
}
void Map::addObstacleCircle(QPointF center, smReal radius)
{
    addObjectCircle(obstacles,center,radius);
}
void Map::closeObstacle()
{
    closeObject(obstacles);
}

QPointF Map::getEntrancePoint()
{
    int polNumber = randomService.randomInt() % entrances.size();

    return entrances[polNumber]->getRandomPointInside();
    
}
