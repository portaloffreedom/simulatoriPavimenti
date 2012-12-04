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


#include "circle.h"

Circle::Circle()
{

}

Circle::Circle(QPointF center, qreal radius)
{
    setCenterAndRadius(center,radius);
}


Circle::~Circle()
{

}


void Circle::draw(QPainter& painter)
{
    painter.drawEllipse(center,radius,radius);

} 

void Circle::setCenterAndRadius(QPointF center, qreal radius)
{
    this->center.setX(center.rx());
    this->center.setY(center.ry());
    this->radius = radius;
}

//TODO controllare bool Circle::isInside(QPoint& point)
bool Circle::isInside(QPointF& point)
{
    qreal distancex = (point.x()-center.x())*(point.x()-center.x());
    qreal distancey = (point.y()-center.y())*(point.y()-center.y());
    qreal distance = sqrt(distancex+distancey);
    if (distance < radius)
	return true;
    else
	return false;
}

