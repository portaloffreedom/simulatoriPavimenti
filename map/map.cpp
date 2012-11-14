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
#include <QPainter>
#include <QPaintEvent>
#include "map.h"

Map::Map()
{   
    this->leftConstraint = 1.0e70;
    this->rightConstraint = -1.0e70;
    this->topConstraint = -1.0e70;
    this->bottomConstraint = 1.0e70;
    
    
    border_v   = new QVector<QPointF>();
    entrance_v = new QVector<QPointF>();
    exit_v     = new QVector<QPointF>();
    obstacles_v = new QVector<QVector<QPointF>*>();
//     obstacles_counter = 0;
    this->show();
}

Map::~Map()
{
//     delete[] border;
//     delete[] entrance;
//     delete[] exit;
//     
//     for (int i=0; i<obstacles_counter; i++) 
// 	delete obstacles[i];
//     delete[] obstacles;
    
    
    
    delete border_v;
    delete entrance_v;
    delete exit_v;
    
    if (obstacles_v != NULL) {
	while (!obstacles_v->empty()) {
	    delete obstacles_v->last();
	    obstacles_v->pop_back();
	}
	delete obstacles_v;
    }
}

void Map::paintEvent(QPaintEvent* event)
{
    float margin = 10;
//     float leftConstraint = this->leftConstraint +50;
//     float rightConstraint = this->rightConstraint *border;
//     float bottomConstraint = this->bottomConstraint +50;
//     float topConstraint = this->topConstraint *border;
    
    //QWidget::paintEvent(event);
    QPainter painter;
    painter.begin(this);
//     painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    
    //disegna ------------------------------------------------------------------
    
    //disegna sfondo
    painter.fillRect(event->rect(),Qt::black);
    
    painter.translate(leftConstraint+margin,rect().bottom()-bottomConstraint-margin);
    painter.scale((rect().width()-margin*2)/(rightConstraint-leftConstraint),-((rect().height()-margin*2)/(topConstraint-bottomConstraint)));
    //disegna bordo
    painter.setPen(QPen(Qt::gray));
    std::cout<<"disegno bordo: \tdimensioni "<<border_v->size()<<std::endl;
    painter.drawPolygon(border_v->data(),border_v->size());
    
    painter.setPen(QPen(Qt::red));
    std::cout<<"disegno uscita:\tdimensioni "<<exit_v->size()<<std::endl;
    painter.drawPolygon(exit_v->data(),exit_v->size());
    
    painter.setPen(QPen(Qt::green));
    std::cout<<"disegno entrata:\tdimensioni "<<entrance_v->size()<<std::endl;
    painter.drawPolygon(entrance_v->data(),entrance_v->size());
    
    painter.setPen(QPen(Qt::white));
    std::cout<<"disegno ostacolo:\tdimensioni "<<obstacles_v->last()->size()<<std::endl;
    painter.drawPolygon(obstacles_v->last()->data(),obstacles_v->last()->size());
    
    
    //fine disegno -------------------------------------------------------------
    painter.restore();
    painter.end();
    
}

void Map::analyzeConstraints(QPointF point)
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


//Border section
void Map::beginBorder()
{
    border_v->reserve(3);
}
void Map::addBorderPoint(QPointF point)
{   
    analyzeConstraints(point);
    border_v->append(point);
//     std::cout<<"Aggiunto un punto al bordo: "<<point.x()<<","<<point.y()<<std::endl;
}
void Map::closeBorder()
{
    //To Optimize Memory
//     border_v->resize(border_v->size());
}

//entrance section
void Map::beginEntrance()
{
    entrance_v->reserve(3);
}
void Map::addEntrancePoint(QPointF point)
{
    analyzeConstraints(point);
    entrance_v->append(point);
}
void Map::closeEntrace()
{
    //To Optimize Memory
//     entrance_v->resize(entrance_v->size());
}

//Exit section
void Map::beginExit()
{
    exit_v->reserve(3);
}
void Map::addExitPoint(QPointF point)
{
    analyzeConstraints(point);
    exit_v->append(point);
}
void Map::closeExit()
{
    //To Optimize Memory
//     exit_v->resize(exit_v->size());
}

//Obstacle section
void Map::beginObstacle()
{
    obstacles_v->append(new QVector<QPointF>);
    obstacles_v->last()->reserve(3);
}
void Map::addObstaclePoint(QPointF point)
{
    analyzeConstraints(point);
    obstacles_v->last()->append(point);
}
void Map::closeObstacle()
{
    //To Optimize Memory
//     obstacles_v->last()->resize(obstacles_v->last()->size());
}






























