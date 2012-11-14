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


#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QEvent>


class Map : public QWidget
{
Q_OBJECT
public:
    Map();
    virtual ~Map();
    
public slots:
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
    
protected:
    void paintEvent(QPaintEvent *event);
    
private:
//     QPointF *border;
//     QPointF *entrance;
//     QPointF *exit;
//     QPointF **obstacles;
//     int obstacles_counter;

    float  leftConstraint;
    float rightConstraint;
    float    topConstraint;
    float  bottomConstraint;

    QVector<QPointF> *border_v;
    QVector<QPointF> *entrance_v;
    QVector<QPointF> *exit_v;
    QVector<QVector<QPointF>*> *obstacles_v;
    
    void analyzeConstraints(QPointF point);
};

#endif // MAP_H
